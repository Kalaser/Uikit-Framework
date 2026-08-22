# API Reference

All public headers are umbrella-included by `uikit/UIKit.h`:

```c
#include "UIKit.h"
```

## Framework lifecycle

| Function          | Description                                        |
| ----------------- | -------------------------------------------------- |
| `UIKit_init()`    | Initialize subsystems (theme, etc.). Call after LVGL + display/input init. |
| `UIKit_process()` | Framework periodic work. Call in the main loop with `lv_timer_handler()`. |

## Core: UIView

| Function | Description |
| --- | --- |
| `UIView_create(parent)` | Create a view (`parent` may be NULL → active screen). |
| `UIView_destroy(view)` | Destroy view, callbacks and LVGL object. |
| `UIView_set_frame(v, x, y, w, h)` / `UIView_get_frame(v, &x, &y, &w, &h)` | Frame management. |
| `UIView_set_position` / `UIView_set_size` | Position / size. |
| `UIView_set_hidden(v, bool)` / `UIView_is_hidden(v)` | Visibility. |
| `UIView_add_subview(parent, child)` | Attach child. |
| `UIView_remove_from_superview(v)` / `UIView_remove_subview(parent, child)` | Detach child (object kept alive). |
| `UIView_get_superview(v)` | Parent pointer. |
| `UIView_set_background_color(v, 0xRRGGBB)` | Background. |
| `UIView_set_border(v, color, width)` | Border. |
| `UIView_set_corner_radius(v, r)` | Corner radius. |
| `UIView_set_opacity(v, 0..255)` | Opacity. |
| `UIView_set_user_interaction(v, bool)` / `UIView_is_user_interaction(v)` | Clickability. |
| `UIView_set_enabled(v, bool)` / `UIView_is_enabled(v)` | Disabled state. |
| `UIView_set_tag(v, ptr)` / `UIView_get_tag(v)` | User pointer. |
| `UIView_add_callback(v, type, cb, ctx)` / `UIView_remove_callback(v, type, cb)` | Events. |
| `UIView_native(v)` | Escape hatch: underlying `lv_obj_t *`. |

## Events

```c
typedef enum {
    UIEVENT_TOUCH_DOWN, UIEVENT_TOUCH_UP, UIEVENT_CLICK,
    UIEVENT_LONG_PRESS, UIEVENT_VALUE_CHANGED, UIEVENT_FOCUS,
    UIEVENT_DEFOCUS, UIEVENT_KEY, UIEVENT_CUSTOM
} UIEventType;

typedef void (*UIEventCallback)(UIEvent *event, void *user_data);
/* UIEvent: type, target, user_data, x, y, value */
```

## Components

All components follow the convention `<Component>_create(parent)` /
`<Component>_destroy(x)` and `set_xxx` / `get_xxx`. Every component struct
embeds `UIView base` as its first member, so `&component->base` can be passed
to any `UIView_*` function.

### UILabel
`UILabel_set_text`, `UILabel_get_text`, `UILabel_set_color(0xRRGGBB)`,
`UILabel_set_alignment(0=left,1=center,2=right)`, `UILabel_set_font(&lv_font_*)`.

### UIButton
`UIButton_set_title` / `UIButton_set_text`, `UIButton_get_title` / `UIButton_get_text`,
`UIButton_set_title_color`, `UIButton_set_background_color`,
`UIButton_on_click` / `UIButton_set_action(cb, ctx)`,
`UIButton_set_event_callback(type, cb, ctx)`.

### UISwitch
`UISwitch_set_on(bool)`, `UISwitch_set_on_instant(bool)`, `UISwitch_is_on`,
`UISwitch_set_knob_color`, `UISwitch_set_on_color`, `UISwitch_on_change`.

### UISlider
`UISlider_set_range(min, max)`, `UISlider_set_value(v)`, `UISlider_get_value`,
`UISlider_set_vertical(bool)`, `UISlider_set_indicator_color`,
`UISlider_set_knob_color`, `UISlider_on_change`.

### UITextField
`UITextField_set_text`, `UITextField_get_text`, `UITextField_set_placeholder`,
`UITextField_set_text_color`, `UITextField_set_max_length`,
`UITextField_set_password_mode(bool)`, `UITextField_set_editable(bool)`,
`UITextField_on_change`.

### UIImageView
`UIImageView_set_image(src)` (lv_img_dsc_t / path / `LV_SYMBOL_*`),
`UIImageView_set_scale(256=100%)`, `UIImageView_set_rotation(0.1°)`,
`UIImageView_set_antialias(bool)`.

### UIScrollView
`UIScrollView_set_vertical_scroll(bool)`, `UIScrollView_set_horizontal_scroll(bool)`,
`UIScrollView_set_scrollbar_visible(bool)`, `UIScrollView_scroll_to_view(subview)`,
`UIScrollView_scroll_to(x, y)`.

### UIProgressView
`UIProgressView_set_range`, `UIProgressView_set_value(v)` / `_instant`,
`UIProgressView_get_value`, `UIProgressView_set_track_color`,
`UIProgressView_set_progress_color`, `UIProgressView_set_vertical(bool)`.

### UIPageControl
`UIPageControl_set_page_count(n)`, `UIPageControl_get_page_count`,
`UIPageControl_set_current_page(i)`, `UIPageControl_get_current_page`,
`UIPageControl_set_current_color`, `UIPageControl_set_page_color`,
`UIPageControl_on_change`.

## Controllers

### UIViewController
Android 风格状态机生命周期，每个回调恰好触发一次、顺序合法：

```text
None → Created → Started → Resumed ⇄ Paused → Stopped → Destroyed
```

```c
typedef struct { UIViewController base; } MyController;
void MyController_create(MyController *c);   /* UIViewController_init(&c->base, destroy_fn); */
void MyController_on_create(UIViewController *self);  /* 回调：设置视图、绑定事件 */
void MyController_on_resume(UIViewController *self);  /* 回调：启动定时器 */
```

字段与回调：`view`、`title`（`UIViewController_set_title/get_title`）、`user_data`、
`state`、`onCreate`/`onStart`/`onResume`/`onPause`/`onStop`/`onDestroy`。
旧 UIKit 钩子（`load_view`、`view_will_appear`、`view_did_appear`、`view_will_disappear`、
`view_did_disappear`、`on_destroy`）标记 deprecated，由状态机自动转发。

触发函数（由导航控制器调用，幂等）：`UIViewController_onCreate/onStart/onResume/onPause/onStop/onDestroy`。

销毁：`UIViewController_deinit(vc)` 触发 onDestroy + 释放视图但**不释放结构体**（供 C++ 成员嵌入）；
`UIViewController_destroy(vc)` = deinit + 释放结构体。
`UIViewController_get_view(vc)` 懒创建根视图（onCreate 时保证就绪）。

### UINavigationController
`UINavigationController_create(root)`、`push(nav, ctrl)`、`pop(nav)`、`pop_to_root(nav)`、
`count`、`top`、`at(i)`、`set_animated(bool)`、`UINavigationController_destroy(nav)`。

生命周期时序：
- **push**：旧栈顶 `onPause → onStop`；新控制器 `onCreate → onStart → onResume`
  （onCreate 幂等，重复 push 不重复创建）。
- **pop**：栈顶 `onPause → onStop`，视图 detach 保留，控制器归还调用方（**不触发 onDestroy**）；
  新栈顶 `onStart → onResume`。
- **nav destroy**：栈上每个控制器 `onPause → onStop → onDestroy` 并释放。

## Graphics

### UITheme
`UITheme_get_default()`, `UITheme_set_default(const UITheme *)`,
`UITheme_reset()`. Fields: `background_color`, `foreground_color`,
`primary_color`, `secondary_color`, `corner_radius`, `font_small/normal/large`.

### UIViewAnimation
`UIView_animate(view, prop, to, duration_ms)` with `UIANIM_PROP_X/Y/WIDTH/HEIGHT/OPACITY`.
`UIView_stop_animations(view)`.

## Layout

### UIStackView
`UIStackView_create(parent)`, `set_axis(HORIZONTAL|VERTICAL)`, `set_spacing(px)`,
`set_padding(px)`, `add_arranged_subview(view)`, `set_alignment(0..3)`.

## Error codes

```c
typedef enum {
    UIKIT_OK = 0, UIKIT_ERROR_INVALID_ARG, UIKIT_ERROR_NO_MEMORY,
    UIKIT_ERROR_NOT_INITIALIZED, UIKIT_ERROR_ALREADY_EXISTS,
    UIKIT_ERROR_NOT_SUPPORTED, UIKIT_ERROR_NOT_FOUND, UIKIT_ERROR_BUSY
} UIKitResult;
```

`UIKitResult_to_string(result)` converts to a human-readable string.
