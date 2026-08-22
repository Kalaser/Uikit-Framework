# Architecture

UIKit-Framework is a **UIKit-style application UI abstraction layer** built on top of
LVGL for embedded systems. It does **not** replace LVGL — it layers a UIKit-style
object model (view hierarchy, lifecycle, navigation, events) on top of LVGL's
rendering/input/animation engine.

## Layered design

```text
+------------------------------------------------------+
|                  Application                         |
+------------------------------------------------------+
|                 UIKit C Core                         |
|   View Tree / Event Dispatcher / Controller /        |
|   Navigation / Theme / Animation / Layout            |
+------------------------------------------------------+
|                       LVGL                           |
|   Object Tree / Input / Rendering / Animation        |
+------------------------------------------------------+
|            Hardware Abstraction Layer                |
|   Display / Touch / Encoder / Timer                  |
+------------------------------------------------------+
|                     Hardware                         |
+------------------------------------------------------+
```

> **Key principle: application code depends on UIKit APIs, not LVGL APIs.**

LVGL becomes the rendering and input engine underneath the framework.

## Module map

| Module            | Path                        | Responsibility                          |
| ----------------- | --------------------------- | --------------------------------------- |
| Core              | `uikit/core/`               | `UIView`, `UIViewRoot`, `UIEvent`, `UIKitResult` |
| Components        | `uikit/components/`         | `UILabel`, `UIButton`, `UISwitch`, `UISlider`, `UITextField`, `UIImageView`, `UIScrollView`, `UIProgressView`, `UIPageControl` |
| Controllers       | `uikit/controllers/`        | `UIViewController`, `UINavigationController` |
| Graphics          | `uikit/graphics/`           | `UITheme`, `UIViewAnimation`           |
| Layout            | `uikit/layout/`             | `UIStackView`                          |
| Platform          | `uikit/platform/`           | `UIKitPlatform` (time/memory/lock/dispatch) |

## Object model

Every UIKit component is a thin wrapper around one LVGL object. The C struct
embeds `UIView` as its first member (single inheritance):

```text
UIView
 ├── UILabel
 ├── UIButton
 ├── UISwitch
 ├── UISlider
 ├── UITextField
 ├── UIImageView
 ├── UIScrollView
 ├── UIProgressView
 ├── UIPageControl
 └── (container for UIStackView / UINavigationController)
```

## Event flow

```text
LVGL Event
    │
    ▼
UIKit Event Adapter   (UIView.c: lvgl_event_handler)
    │
    ▼
UIEventType          (framework-level event codes)
    │
    ▼
Application Callback (UIEventCallback)
```

Each `UIEventType` is wired to its LVGL event exactly once per view
(`registered_events` bitmask), so multiple callbacks for the same type
do not duplicate LVGL registrations.

## Controller lifecycle

```text
create → loadView → viewWillAppear → viewDidAppear → ACTIVE
        → viewWillDisappear → viewDidDisappear → destroy
```

`UINavigationController` drives these hooks on push/pop/pop-to-root.
Ownership rules:

- **Push** transfers controller ownership to the navigation controller.
- **Pop** returns ownership to the caller; the popped view is detached to the
  active screen so it stays valid.
- **Nav destroy** destroys every controller still on the stack, then the container.

## Threading

LVGL requires serialized access. Worker threads must not touch UIKit/LVGL
objects directly; use `UIKit_dispatch()` (platform adapter) to hop onto the
UI thread. See `docs/porting.md`.
