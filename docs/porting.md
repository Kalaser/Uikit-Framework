# Porting Guide

UIKit-Framework depends on LVGL plus a **small platform adapter**. To bring up a
new board, complete the adapter below, then initialize LVGL as usual.

## Platform adapter (`uikit/platform/UIKitPlatform.h`)

The core framework only calls these functions. Defaults target bare metal /
host builds and wrap the C standard library.

| Function | Default | When to override |
| --- | --- | --- |
| `uint32_t UIKit_get_tick_ms(void)` | `lv_tick_get()` | Already correct on most targets. |
| `void *UIKit_malloc(size_t)` | `malloc` | Static pools / no-heap builds. |
| `void UIKit_free(void *)` | `free` | Match your allocator. |
| `void *UIKit_calloc(n, size)` | `calloc` | Match your allocator. |
| `void UIKit_lock(void)` / `void UIKit_unlock(void)` | no-op | Multi-threaded UI access. |
| `void UIKit_dispatch(fn, ctx)` | direct call | RTOS message-queue hop to UI thread. |

To provide custom implementations:

1. Define `UIKIT_PLATFORM_CUSTOM` before including `UIKitPlatform.h`
   (e.g. in your build flags).
2. Provide every function listed above yourself.

## Display / input

UIKit does not own the display or input drivers — LVGL does. Initialization
sequence in your `main()`:

```c
lv_init();

/* 1. Register your display driver (and input driver) with LVGL. */
my_display_init();   /* lv_disp_drv_register(...) */
my_touch_init();     /* lv_indev_drv_register(...) */

/* 2. Initialize the framework. */
UIKit_init();

/* 3. Build UI with UIKit APIs. */
UIKit_setRootViewController(...);  /* see examples/ */

/* 4. Main loop. */
while (1) {
    lv_tick_inc(my_tick_ms());   /* or use lv_tick_set_cb-style platform tick */
    lv_timer_handler();
    UIKit_process();
}
```

## Threading model

LVGL requires serialized access to the object tree. Recommended pattern:

```text
Worker thread ── UIKit_dispatch(update_fn, data) ──► UI thread (LVGL)
```

Implement `UIKit_dispatch()` to post `func`/`user_data` to the UI task's queue.
On bare metal, call the function directly (the default).

## Feature configuration

Optional compile-time switches (future work, reserved for Phase 3):

```c
#define UIKIT_ENABLE_NAVIGATION    1
#define UIKIT_ENABLE_ANIMATION     1
#define UIKIT_ENABLE_THEME         1
#define UIKIT_USE_HEAP             1
```

## LVGL version

Currently targets **LVGL 8.x** (verified against 8.3.11). All LVGL interaction
is isolated inside `uikit/` — application code never includes `lvgl.h` directly
(only `UIKit.h`).

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build          # run tests
```

Embedded targets: add `uikit` as a subdirectory or compile `uikit/**/*.c`
together with your LVGL sources. See `README.md` §24 (Build Systems).
