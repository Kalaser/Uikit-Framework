# C++ Wrapper (uikit/cpp/UIKit.hpp)

Header-only, C++11. Include `UIKit.hpp` and link against the C `uikit` library:

```cpp
#include "UIKit.hpp"
```

All classes live in `namespace uikit`. The C API remains fully available
(global namespace, `extern "C"`).

## Quick example

```cpp
class MainViewController : public uikit::UIViewController {
public:
    void onCreate() override {
        auto view = getView();                    // borrowed UIView
        view.setBackgroundColor(0xF2F2F7);

        auto title = uikit::UILabel::create();    // value semantics (move-only)
        title.setText("Home");
        title.setFrame(0, 20, 320, 30);
        view.addSubview(title);

        auto button = uikit::UIButton::create();
        button.setTitle("Start");
        button.onClick([](uikit::UIButton *) { printf("Start clicked\n"); });
        view.addSubview(button);
    }
};

int main(void) {
    lv_init(); /* + display/input drivers */
    ::UIKit_init();

    uikit::UINavigationController nav(new MainViewController());  // owns it

    while (1) { lv_timer_handler(); ::UIKit_process(); }
}
```

## Class map

| C++ class | Wraps | Highlights |
| --- | --- | --- |
| `uikit::UIView` | `::UIView` | frame, visibility, hierarchy, appearance, tag, `animate()` |
| `uikit::UILabel` | `::UILabel` | `setText` / `text`, `setColor`, `setAlignment` |
| `uikit::UIButton` | `::UIButton` | `setTitle`, `onClick(std::function<void(UIButton*)>)` |
| `uikit::UISwitch` | `::UISwitch` | `setOn`, `isOn`, `onChange(std::function<void(bool)>)` |
| `uikit::UISlider` | `::UISlider` | `setRange`, `setValue`, `value`, `onChange(std::function<void(int)>)` |
| `uikit::UITextField` | `::UITextField` | `setText`, `setPlaceholder`, `setPasswordMode`, `onChange` |
| `uikit::UIImageView` | `::UIImageView` | `setImage`, `setScale`, `setRotation` |
| `uikit::UIScrollView` | `::UIScrollView` | `setVerticalScroll`, `scrollTo` |
| `uikit::UIProgressView` | `::UIProgressView` | `setRange`, `setValue`, `value` |
| `uikit::UIPageControl` | `::UIPageControl` | `setPageCount`, `setCurrentPage`, `onChange` |
| `uikit::UIStackView` | `::UIStackView` | `setAxis`, `setSpacing`, `addArrangedSubview` |
| `uikit::UIViewController` | `::UIViewController` | virtual lifecycle, `getView`, `setTitle` |
| `uikit::UINavigationController` | `::UINavigationController` | `push`, `pop`, `popToRoot`, `top`, `at` |

## Application / Activity structure

For Android-style apps, treat each page as a `uikit::UIViewController` Activity
and keep the process/app lifecycle in a small application object. See
`docs/app-lifecycle.md` for a complete launcher-style skeleton with
`Application.onCreate/onForeground/onBackground/onDestroy`, page
`onCreate/onStart/onResume/onPause/onStop/onDestroy`, and ownership rules for
`startActivity()` / `finish()`.

## Lifecycle (virtual dispatch)

The six C callbacks map to virtual functions through static thunks
(`user_data` holds the C++ `this`):

```cpp
virtual void onCreate();   // once, first entry into a nav stack
virtual void onStart();    // visible
virtual void onResume();   // interactive (current screen)
virtual void onPause();    // lost focus
virtual void onStop();     // invisible
virtual void onDestroy();  // real teardown
```

`onDestroy` is only guaranteed to dispatch virtually **while the object is
alive** — i.e. it is triggered by the navigation controller (on teardown) or
by an explicit `destroy()`. The C++ destructor is a fallback: if the C object
was never torn down, it frees it; but because virtual dispatch is statically
bound inside a destructor, subclass `onDestroy` overrides may not run in that
fallback path. Prefer letting the framework drive the lifecycle.

## Ownership rules

- **Move-only**: wrappers own their C object and free it on destruction.
  Copying is disabled.
- **Controllers + navigation**: controllers passed to a
  `UINavigationController` **must be heap-allocated (`new`)**. The nav takes
  ownership, fires `onDestroy` (virtual, object alive) and `delete`s them on
  its own destruction.
- **pop() / popToRoot()** return ownership to the caller — the caller must
  `delete` the controllers (or push them again).
- `getView()` returns a **borrowed** wrapper (does not free); the controller
  owns its root view.
- Children added via `addSubview` are owned by their own wrappers; destroy
  children before parents (same rule as the C API).

## Move semantics & events

Wrappers are movable. Components with event callbacks (`UIButton`, `UISwitch`,
`UISlider`, `UITextField`, `UIPageControl`) re-bind the C event `user_data` on
move, so callbacks keep working after a move:

```cpp
uikit::UIButton a;
a.onClick(...);
uikit::UIButton b = std::move(a);   // callbacks still fire on b
```

## Embedding

Header-only — no extra sources to compile. Add `uikit/cpp` to your include
path (already exported by the CMake `uikit` target) and link `uikit` + `lvgl`.
C++ exceptions/RTTI are not required by the wrapper.
