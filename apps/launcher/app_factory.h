/**
 * @file app_factory.h
 * @brief Standalone-app registry for the launcher.
 *
 * Every app lives in its own directory under apps/launcher/apps/ and
 * exposes a create() factory returning a heap-allocated
 * uikit::UIViewController. The launcher home screen only knows the
 * AppMeta table — it never includes app internals.
 */

#ifndef LAUNCHER_APP_FACTORY_H
#define LAUNCHER_APP_FACTORY_H

#include "UIKit.hpp"

struct AppMeta {
    const char *name;
    const char *icon;
    uint32_t    color;
    uikit::UIViewController *(*create)(uikit::UINavigationController *nav);
};

/* ── App factories (implemented in apps/<name>/<name>_app.cpp) ───────────── */
uikit::UIViewController *phone_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *camera_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *gallery_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *music_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *weather_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *maps_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *clock_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *calc_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *files_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *mail_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *store_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *video_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *settings_app_create(uikit::UINavigationController *nav);

/* ── Registry tables (defined in app_registry.cpp) ────────────────────────── */
extern const AppMeta kLauncherPage1[6];
extern const AppMeta kLauncherPage2[6];
extern const AppMeta kLauncherDock[4];

#endif /* LAUNCHER_APP_FACTORY_H */
