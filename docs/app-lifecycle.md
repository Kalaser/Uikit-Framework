# Application / Activity lifecycle design

This framework should model an app as a small lifecycle owner plus one or more
page controllers.  The existing `UIViewController` state machine is the page
(`Activity`) lifecycle, and `UINavigationController` is the task/back-stack that
moves pages between foreground, background, and destruction.

## Roles

```text
Application
  ├── onCreate()       register pages, create the root page
  ├── onForeground()   app task becomes visible
  ├── onBackground()   app task is hidden but retained
  └── onDestroy()      app task exits and releases resources

Activity / page (UIViewController)
  ├── onCreate()       build UI once
  ├── onStart()        page becomes visible
  ├── onResume()       page is the focused interactive page
  ├── onPause()        page loses focus
  ├── onStop()         page is no longer visible
  ├── onRestart()      optional app-level convention before Stopped → Started
  └── onDestroy()      release page-owned resources
```

UIKit currently exposes the page layer as `UIViewController`.  Projects that
prefer Android naming can use an `Activity` class/typedef in their app code, but
it should keep the same state transitions so navigation remains deterministic.

## Standard transitions

```text
Launch app:
  Application.onCreate
  RootActivity.onCreate → onStart → onResume

startActivity(Next):
  Current.onPause → Current.onStop
  Next.onCreate → Next.onStart → Next.onResume

finish() / back:
  Current.onPause → Current.onStop
  previous.onStart → previous.onResume
  Current.onDestroy only when the popped page owner deletes it

moveToBackground():
  Top.onPause → Top.onStop
  Application.onBackground

returnToForeground():
  Application.onForeground
  Top.onStart → Top.onResume

Exit app:
  Top...Root onPause → onStop → onDestroy
  Application.onDestroy
```

`onRestart()` is intentionally an application-level convenience hook rather than
a separate C state today.  If an app needs it, call it immediately before
`UIViewController_onStart()` when a stopped page is being brought back.

## C++ app skeleton

The skeleton below follows the same shape as an Android-style launcher activity:
`Application` registers/creates the first page, each page builds its UI in
`onCreate()`, starts other pages by pushing onto the navigation stack, and uses
`onResume()` / `onStop()` for foreground/background behavior.

```cpp
#include "UIKit.hpp"

#include <memory>
#include <vector>

class Activity : public uikit::UIViewController {
public:
    explicit Activity(uikit::UINavigationController **nav) : m_nav(nav) {}

protected:
    void startActivity(Activity *activity) {
        if (m_nav && *m_nav && activity) {
            (*m_nav)->push(activity);      // nav owns activity until pop()
        }
    }

    Activity *finish() {
        if (!m_nav || !*m_nav) {
            return nullptr;
        }
        auto *popped = (*m_nav)->pop();    // caller owns popped page again
        return static_cast<Activity *>(popped);
    }

private:
    uikit::UINavigationController **m_nav;
};

class SettingsActivity : public Activity {
public:
    using Activity::Activity;

    void onCreate() override {
        auto root = getView();
        root.setBackgroundColor(0x202124);

        mTitle.reset(new uikit::UILabel(&root));
        mTitle->setText("Settings");
        mTitle->setFrame(16, 16, 240, 32);
        mTitle->setColor(0xFFFFFF);
    }

    void onDestroy() override {
        mTitle.reset();
    }

private:
    std::unique_ptr<uikit::UILabel> mTitle;
};

class HomeActivity : public Activity {
public:
    using Activity::Activity;

    explicit HomeActivity(uikit::UINavigationController **nav) : Activity(nav), mNav(nav) {}

    void onCreate() override {
        auto root = getView();
        root.setBackgroundColor(0x111827);

        mOpenSettings.reset(new uikit::UIButton(&root));
        mOpenSettings->setTitle("settings");
        mOpenSettings->setFrame(20, 20, 120, 44);
        mOpenSettings->onClick([this](uikit::UIButton *) {
            startActivity(new SettingsActivity(mNav));
        });
    }

    void onResume() override {
        // Restart timers, sensor subscriptions, or refresh page data here.
    }

    void onStop() override {
        // Stop expensive work while another page covers this one.
    }

    void onDestroy() override {
        mOpenSettings.reset();
    }

private:
    uikit::UINavigationController **mNav = nullptr;
    std::unique_ptr<uikit::UIButton> mOpenSettings;
};

class LauncherApplication {
public:
    void onCreate() {
        mNav.reset(new uikit::UINavigationController(new HomeActivity(&mNavRaw)));
        mNavRaw = mNav.get();
    }

    void onForeground() {}
    void onBackground() {}

    void onDestroy() {
        mNavRaw = nullptr;
        mNav.reset();
    }

private:
    uikit::UINavigationController *mNavRaw = nullptr;
    std::unique_ptr<uikit::UINavigationController> mNav;
};
```

## Ownership rules for pages

- `startActivity(new Page(...))`: navigation takes ownership of the new page.
- `pop()` / `finish()`: ownership returns to the caller; delete the returned page
  after the input callback is finished, or push it again.
- Page-owned views should be `std::unique_ptr` members and reset in
  `onDestroy()` before the root view is destroyed.
- Avoid deleting the current page directly inside its own LVGL callback; defer
  deletion by one UI tick if the page was popped from a click handler.

## Recommended source layout

```text
apps/<app>/
├── <app>_main.cpp          # Application subclass / main loop
├── ActivityBase.h          # app-local Activity helper if Android naming is wanted
├── pages/
│   ├── HomeActivity.h
│   ├── HomeActivity.cpp
│   ├── SettingsActivity.h
│   └── SettingsActivity.cpp
└── services/               # package manager, storage, networking, etc.
```

Keep page files independent: a page should depend on framework APIs and its own
services, not on launcher internals.  Cross-page navigation should go through a
small route/factory layer instead of directly including every page everywhere.
