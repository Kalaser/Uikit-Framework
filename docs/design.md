# UIKit-Framework 设计文档

> 版本：0.3.0（2026-08-22）
> 定位：一份面向实现的设计说明，解释"为什么这样设计"。API 用法见 [`api.md`](api.md) 与 [`cpp.md`](cpp.md)，移植方法见 [`porting.md`](porting.md)。

---

## 1. 概述

**UIKit-Framework** 是一个构建在 LVGL 之上的、UIKit 风格的**应用层 UI 抽象框架**，面向嵌入式系统。它不替代 LVGL，而是把 UIKit 的对象模型（视图树、生命周期、导航、事件）翻译成 LVGL 原语。

```text
LVGL            = 渲染 + 输入 + 动画引擎
UIKit-Framework = 对象模型 + 生命周期 + 导航 + 事件 + UI 抽象
Application     = 产品逻辑 + 状态 + 业务行为
```

**核心原则**：应用代码只依赖 UIKit API，不直接包含 `lvgl.h`。LVGL 的所有交互被隔离在 `uikit/` 内部。

### 1.1 设计目标

| 目标 | 手段 |
| --- | --- |
| 轻量 | C 核心，无异常/RTTI/STL 依赖；组件与 LVGL 对象一一对应，零中间层开销 |
| 可移植 | 仅依赖极小的平台接口（`UIKitPlatform`） |
| 一致性 | 统一 `<Component>_create/destroy/set_xxx/get_xxx` 命名约定 |
| 可组合 | 视图树 + 导航栈 + 事件回调，嵌入式也能复用页面 |
| 可测试 | 无头显示驱动 + 单元测试 + 冒烟模式 |

---

## 2. 架构分层

```text
+------------------------------------------------------+
|                  Application (Launcher / Apps)       |
+------------------------------------------------------+
|              UIKit C++ Wrapper (uikit/cpp)           |
|   RAII / 虚函数生命周期 / std::function 事件          |
+------------------------------------------------------+
|                    UIKit C Core                      |
|   View Tree / Event Dispatcher / Controller /        |
|   Navigation / Theme / Animation / Layout            |
+------------------------------------------------------+
|                       LVGL                           |
|   Object Tree / Input / Rendering / Animation        |
+------------------------------------------------------+
|         Hardware Abstraction / 平台适配层             |
|   UIKitPlatform (time/memory/lock/dispatch)          |
|   win_driver (Windows GDI 显示/输入，仅 launcher)     |
+------------------------------------------------------+
```

### 2.1 模块地图

| 模块 | 路径 | 职责 |
| --- | --- | --- |
| Core | `uikit/core/` | `UIView`、`UIViewRoot`、`UIEvent`、`UIKitResult` |
| Components | `uikit/components/` | 9 个控件：Label/Button/Switch/Slider/TextField/ImageView/ScrollView/ProgressView/PageControl |
| Controllers | `uikit/controllers/` | `UIViewController`（状态机生命周期）、`UINavigationController`（栈导航） |
| Graphics | `uikit/graphics/` | `UITheme` 主题、`UIViewAnimation` 动画抽象 |
| Layout | `uikit/layout/` | `UIStackView`（映射 LVGL flex） |
| C++ | `uikit/cpp/` | header-only 包装（namespace `uikit`） |
| Platform | `uikit/platform/` | 平台抽象（默认裸机实现） |

---

## 3. 核心设计决策

### 3.1 对象模型：C 单继承

每个组件是一个 C 结构体，**首成员嵌入 `UIView base`**（单继承）：

```c
struct UIButton { UIView base; lv_obj_t *label; };
```

- `&button->base` 可直接传给任意 `UIView_*` 函数（首成员地址 == 结构体地址）
- 每个 UIKit 对象与一个 `lv_obj_t` **一一对应**——没有额外渲染中间层
- 为什么 C 而不是 C++：嵌入式工具链最普及、无 ABI/异常开销、可被任意语言包装

### 3.2 生命周期状态机（Android 风格）

`UIViewController` 采用 Android Activity 风格的生命周期，由**状态机**驱动：

```text
None → Created → Started → Resumed ⇄ Paused → Stopped → Destroyed
```

| 回调 | 触发时机 | 典型用途 |
| --- | --- | --- |
| `onCreate` | 首次入栈，恰好一次 | 初始化、构建视图 |
| `onStart` | 可见（无焦点） | 准备动画 |
| `onResume` | 完全可交互（当前屏） | 启动定时器/传感器 |
| `onPause` | 失去焦点（被覆盖） | 暂停动画、保存轻量状态 |
| `onStop` | 完全不可见 | 释放部分资源 |
| `onDestroy` | 真实销毁，恰好一次 | 释放全部资源 |

**为什么用状态机**（而不是裸回调）：

1. **幂等**：`onCreate` 只在 `None→Created` 迁移时触发。同一控制器重复 push 不会重复创建。
2. **防乱序**：非法迁移（如未 `onStart` 就 `onResume`）被自动忽略。调用方可以无条件调用触发函数。
3. **恢复路径**：`onStart` 接受 `Created|Stopped`、`onResume` 接受 `Started|Paused`，天然支持"重新 push 已 pop 的控制器"。

**触发语义（由导航控制器编排）**：

```text
push(B)：  A.onPause → A.onStop → B.onCreate → B.onStart → B.onResume
pop()：    B.onPause → B.onStop → (B 视图 detach 到屏幕) → A.onStart → A.onResume
nav 销毁： 栈上每个控制器 onPause → onStop → onDestroy
```

**兼容层**：旧的 UIKit hooks（`viewWillAppear` 等）保留并标记 deprecated，由状态机**自动转发**（`onStart→view_will_appear`、`onResume→view_did_appear`、`onPause→view_will_disappear`、`onStop→view_did_disappear`、`onDestroy→on_destroy`），已有代码零改动迁移。

### 3.3 事件系统

```text
LVGL Event → UIKit Event Adapter → UIEventType → 应用回调
```

- `UIEventType` 枚举屏蔽 LVGL 事件码
- `UIEvent` 携带 `type/target/user_data/x/y/value`
- **每种事件类型每个视图只注册一次 LVGL 处理器**（`registered_events` 位掩码去重）——同类型多个回调不会重复触发
- 对程序化发送事件（无活跃输入设备）做了 NULL 安全保护

### 3.4 所有权模型

嵌入式需要**显式所有权**。规则：

| 对象 | 所有权 |
| --- | --- |
| 视图 | 谁创建谁释放；子视图先于父视图销毁 |
| 控制器 | `UINavigationController` **push 时接管**；`pop` 时归还调用方（视图 detach 到屏幕保持存活，**不触发 onDestroy**） |
| `onDestroy` | 只由 `UIViewController_destroy` 触发，恰好一次 |
| 导航销毁 | 先销毁栈上控制器（触发 onDestroy）再删容器，避免 lv_obj 级联删除导致双重释放 |

**deinit / destroy 分离**：

- `UIViewController_deinit(vc)`：触发 onDestroy + 释放根视图，**不 free 结构体**（供 C++ 成员嵌入场景）
- `UIViewController_destroy(vc)`：deinit + 释放结构体

**释放顺序的关键**：C 层 `deinit` 先调 onDestroy 回调、再销毁视图。因此 C++ 控制器在 `onDestroy` 中先释放子视图（unique_ptr 成员 reset），视图销毁时无残留子对象，安全。

### 3.5 平台抽象

```c
uint32_t UIKit_get_tick_ms(void);    // 时间
void *UIKit_malloc/free/calloc();    // 内存
void UIKit_lock(void);               // 同步（默认空操作）
void UIKit_dispatch(fn, ctx);        // UI 线程派发
```

定义 `UIKIT_PLATFORM_CUSTOM` 可整体替换，适配裸机 / FreeRTOS / Zephyr / ESP-IDF / Linux 等。

---

## 4. C++ 包装层设计（uikit/cpp/UIKit.hpp）

header-only、C++11、namespace `uikit`。

### 4.1 命名冲突规避

C 结构体 `UIViewController` 与 C++ 类同名 → 全部类放进 `namespace uikit`，C 类型以 `::UIViewController` 引用，互不干扰。

### 4.2 生命周期桥接（thunk）

```cpp
class UIViewController {
    UIViewController() {
        m_vc = ...;                    // C 结构体（UIKit_calloc）
        m_vc->user_data = this;        // 桥接关键
        m_vc->onCreate = &thunk_create;// 静态 thunk
    }
    static void thunk_create(::UIViewController *vc) {
        static_cast<UIViewController*>(vc->user_data)->onCreate();  // 虚函数派发
    }
    virtual void onCreate() {}          // 子类 override
};
```

**C 回调 → C++ 虚函数的双向桥**。move 时自动重绑 `user_data` 到新对象。

### 4.3 所有权与 RAII

- **move-only**：禁拷贝，move 转移 C 指针（源置空）
- 事件组件（Button/Switch/Slider/TextField/PageControl）在 move 时**重绑定 C 回调的 user_data**
- 导航控制器：析构时**先** C 层销毁（触发 onDestroy 虚函数，对象仍存活）**再** delete C++ 对象

### 4.4 已知陷阱与约定

**C++ 析构函数中虚函数是静态绑定的**：在 `~UIViewController` 里调 `onDestroy()` 不会派发到子类。因此约定：

- `onDestroy` 必须由导航控制器或显式 `destroy()` 在**对象存活时**触发
- C++ 析构函数只是兜底（释放 C 资源，不保证虚函数派发）
- `destroy()` 后 `m_vc` 置空，析构跳过，幂等

---

## 5. Windows Launcher 设计（apps/launcher/）

### 5.1 平台后端 win_driver（GDI）

| 决策 | 依据 |
| --- | --- |
| 16bpp RGB565 DIB 双缓冲 | 与 LVGL `LV_COLOR_DEPTH 16` 完全一致，flush 直接 memcpy，零色彩转换 |
| **partial 分块渲染**（单 buffer 1/8 屏） | `full_refresh` 双缓冲在 GDI 下实测出现画面错位/下沉——标准嵌入式模式最可靠 |
| `SetProcessDPIAware()` | MinGW 无 DPI manifest，默认被系统缩放导致坐标错乱；强制 1:1 像素映射 |
| 鼠标 pointer + 键盘 keypad 双 indev | Windows 键盘直接驱动 LVGL textarea 输入 |
| 自适应窗口 | `WM_SIZE` → 重建 DIB（stride 用动态宽度）→ `lv_disp_drv_update` 更新 LVGL 分辨率 → 回调应用 relayout |
| `--smoke N` | 跑 N 帧后自动退出，供 CI/自动化验证 |

### 5.2 Launcher 桌面

```
┌─────────────────────────┐
│ 状态栏  时间 · WiFi · 电量 │  ← 固定顶部
├─────────────────────────┤
│ 搜索框                  │
├─────────────────────────┤
│ 桌面 UIScrollView（2 页横滑）│  ← 占中间剩余空间
│  3 列 app 图标网格 + snap │
│  UIPageControl 页指示    │
├─────────────────────────┤
│ Dock：4 个快捷 app       │  ← 固定底部（88px）
└─────────────────────────┘
```

**响应式布局**：`relayout()` 读取 `lv_disp_get_hor_res/ver_res` 重算全部 frame（状态栏/dock 钉住、桌面弹性、3 列按宽度自适应），在 `onCreate` 与窗口 `WM_SIZE` 时调用。

### 5.3 独立 App 插件架构

```text
apps/launcher/
├── app_common.h       # 共享工具（返回按钮/延迟销毁/标题）
├── app_factory.h      # AppMeta 接口
├── app_registry.cpp   # 注册表：kLauncherPage1/2 + kLauncherDock
├── launcher.cpp       # 桌面 + main（只依赖注册表）
└── apps/<name>/       # 每个 app 独立目录 + 独立设计
```

```c
struct AppMeta {
    const char *name;
    const char *icon;                    // LV_SYMBOL_*
    uint32_t color;
    uikit::UIViewController *(*create)(uikit::UINavigationController *nav);
};
```

- 每个 app 是一个独立 `UIViewController` 子类 + `create()` 工厂，拥有自己的 UI 与交互
- **launcher 桌面不知道任何 app 内部实现**——新增 app = 新建目录 + 注册一行
- `--apps` 自检模式：程序化依次 push/pop 全部注册条目，验证 onCreate/onDestroy/所有权（CI 友好）

### 5.4 App 内子视图生命周期约定

- 子视图用 `std::unique_ptr` 成员持有
- 在 `onDestroy` 里 `reset()` 全部子视图（C 层 deinit 顺序保证：先回调后删视图）
- 返回按钮 `pop()` 后经 `lv_timer` **延迟一帧** destroy+delete（避免点击回调内 use-after-free）

---

## 6. 关键实现细节与踩坑记录

| 坑 | 现象 | 根因 | 解决 |
| --- | --- | --- | --- |
| lv_obj 默认白底 | 深色壁纸被白块覆盖 | LVGL 8.3 默认 `bg_opa=COVER` 白色 | 显式设置 bg 颜色；nav container 设透明 |
| `UIView_get_frame` 返回 0 | 布局断言失败 | `lv_obj_get_x/y/width/height` 读的是布局后缓存，`lv_timer_handler` 前恒为 0 | 改用 style getter（`lv_obj_get_style_x` 等） |
| `uikit::UIView(::UIView*, bool)` 误用 | 整个 UI 被"压"到屏幕底部 | 把父 view 指针当子视图传入，包装对象改写了父对象自身 | 子视图一律用带 `UIView* parent` 的构造函数 |
| full_refresh 双缓冲错乱 | 内容下沉/残影 | GDI 与 LVGL swap 语义冲突 | 改用 partial 分块渲染 |
| LVGL 8.3 API 差异 | 编译/链接失败 | v9 API 名混入 | `lv_anim_set_time`（非 `set_duration`）；无 `LV_FLEX_ALIGN_STRETCH`（用 START） |
| DPI 缩放 | 窗口被放大、坐标错乱 | MinGW 无 manifest | `#define _WIN32_WINNT 0x0600` + `SetProcessDPIAware()` |
| resize 后 stride 错 | 图像错位 | DIB 行跨度仍用旧常量 | flush 使用 `wd->width` 动态 stride |

---

## 7. 测试与验证

| 层 | 内容 | 命令 |
| --- | --- | --- |
| C 单元测试 | 24 项：核心/组件/控制器/状态机/主题/布局/兼容转发 | `build_tdm/UIKitTests.exe` |
| C++ 单元测试 | 10 项：虚函数派发/所有权/move 重绑定/RAII | `build_tdm/UIKitCppTests.exe` |
| ctest | 聚合以上 | `ctest --test-dir build_tdm` |
| Launcher 冒烟 | 真实窗口跑 N 帧自动退出 | `UIKitLauncher.exe --smoke 180` |
| Launcher app 自检 | 程序化启动全部 app 验证生命周期 | `UIKitLauncher.exe --apps` |
| 无头渲染 | 所有测试/示例共用 no-op flush 驱动 | — |

---

## 8. 路线图对照（README Phase 1–5）

| Phase | 内容 | 状态 |
| --- | --- | --- |
| 1 核心 | UIView/UILabel/UIButton/UISwitch/UISlider/UIImageView/UIViewController/UINavigationController | ✅ 已实现（含 TextField/ScrollView/ProgressView/PageControl） |
| 2 基础设施 | 事件分发/生命周期/主题/布局/动画/线程派发/内存管理 | ✅ 已实现（UITheme/UIStackView/UIViewAnimation/UIKitPlatform） |
| 3 嵌入式优化 | 静态内存池/编译期裁剪/无堆模式/性能剖析 | ⏳ 待做 |
| 4 高级组件 | UITableView/UICollectionView/UIAlertController/UITabBarController 等 | ⏳ 待做 |
| 5 兼容性 | LVGL 9 适配/渲染后端抽象 | ⏳ 待做 |

---

## 9. 设计哲学回顾

1. **不替代 LVGL，只做抽象**——所有 UIKit 对象都能 `UIView_native()` 访问底层 lv_obj（逃生舱）。
2. **C 为核心，C++ 可选**——C API 是地基，C++ 包装只是薄壳。
3. **状态机胜过回调约定**——生命周期由框架保证，不依赖调用方自觉。
4. **显式所有权**——嵌入式没有 GC，每个对象都有明确的 owner 与销毁时机。
5. **每个组件一个用例**——launcher 的 13 个 app 就是框架能力的活文档。
