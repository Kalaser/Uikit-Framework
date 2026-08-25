/**
 * @file app_factory.h
 * @brief Standalone-app registry for the launcher (Android-style app list).
 *
 * Every app lives in its own directory under apps/launcher/apps/ and exposes
 * a create() factory. The launcher only knows the AppDescriptor table —
 * like Android's package list: an app is described by id / name / icon /
 * category / launch target, and "launching" it is equivalent to firing an
 * intent at its controller factory.
 *
 * System UI pages (drawer / recents / quick settings / long-press menu) are
 * built by the launcher itself and live under apps/launcher/system/.
 */

#ifndef LAUNCHER_APP_FACTORY_H
#define LAUNCHER_APP_FACTORY_H

#include "UIKit.hpp"
#include <vector>

/** App metadata — Android "package" equivalent. */
struct AppDescriptor {
    const char *id;        /**< stable id, e.g. "phone", "music" */
    const char *name;      /**< display name */
    const char *icon;      /**< LV_SYMBOL_* glyph */
    uint32_t    color;     /**< tile color */
    const char *category;  /**< "comms" | "media" | "tools" | "system" */
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
uikit::UIViewController *settings_xml_app_create(uikit::UINavigationController *nav);
uikit::UIViewController *terminal_app_create(uikit::UINavigationController *nav);

/* ── Registry tables (defined in app_registry.cpp) ────────────────────────── */
extern const AppDescriptor kLauncherApps[15];   /**< full installed-app list */
extern const AppDescriptor *kLauncherPage1[6];  /**< home page 1 (pointers) */
extern const AppDescriptor *kLauncherPage2[8];  /**< home page 2 (pointers) */
extern const AppDescriptor *kLauncherDock[4];   /**< dock shortcuts */

/* ── System UI factories (apps/launcher/system/) ──────────────────────────── */

/** App drawer — lists every installed app grouped by category (Android). */
uikit::UIViewController *launcher_drawer_create(uikit::UINavigationController *nav);

/** Recent apps — re-launch previously opened apps (Android Recents / iOS
 *  app switcher). Takes the most-recent-first list. */
uikit::UIViewController *launcher_recents_create(
    uikit::UINavigationController *nav,
    const std::vector<const AppDescriptor *> &recents);

/** Quick settings — system toggles pulled down from the status bar. */
uikit::UIViewController *launcher_quicksettings_create(uikit::UINavigationController *nav);

/** Long-press app info menu. */
uikit::UIViewController *launcher_app_menu_create(uikit::UINavigationController *nav,
                                                  const AppDescriptor &app);

#endif /* LAUNCHER_APP_FACTORY_H */
