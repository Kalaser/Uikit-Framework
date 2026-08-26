/**
 * @file app_registry.cpp
 * @brief Aggregates every standalone app into the launcher registry.
 */

#include "app_factory.h"

#include <cstdio>

const AppDescriptor kLauncherApps[15] = {
    { "phone",   "Phone",   LV_SYMBOL_CALL,      0x34D399, "comms", phone_app_create },
    { "camera",  "Camera",  LV_SYMBOL_EYE_OPEN,  0x60A5FA, "media", camera_app_create },
    { "gallery", "Gallery", LV_SYMBOL_IMAGE,     0xF472B6, "media", gallery_app_create },
    { "music",   "Music",   LV_SYMBOL_AUDIO,     0xA78BFA, "media", music_app_create },
    { "weather", "Weather", LV_SYMBOL_TINT,      0x38BDF8, "tools", weather_app_create },
    { "maps",    "Maps",    LV_SYMBOL_GPS,       0x4ADE80, "tools", maps_app_create },
    { "clock",   "Clock",   LV_SYMBOL_REFRESH,   0xFBBF24, "tools", clock_app_create },
    { "calc",    "Calc",    LV_SYMBOL_EDIT,      0x2DD4BF, "tools", calc_app_create },
    { "files",   "Files",   LV_SYMBOL_DIRECTORY, 0xFB923C, "tools", files_app_create },
    { "mail",    "Mail",    LV_SYMBOL_ENVELOPE,  0x818CF8, "comms", mail_app_create },
    { "store",   "Store",   LV_SYMBOL_DOWNLOAD,  0xF87171, "tools", store_app_create },
    { "video",   "Video",   LV_SYMBOL_VIDEO,     0xE879F9, "media", video_app_create },
    { "settings","Settings",LV_SYMBOL_SETTINGS,  0x334155, "system", settings_app_create },
    { "setxml",  "Settings XML", LV_SYMBOL_SETTINGS, 0x2DD4BF, "system", settings_xml_app_create },
    { "terminal","Terminal", LV_SYMBOL_KEYBOARD, 0x0E0E11, "tools", terminal_app_create },
};

const AppDescriptor *kLauncherPage1[6] = {
    &kLauncherApps[0], &kLauncherApps[1], &kLauncherApps[2],
    &kLauncherApps[3], &kLauncherApps[4], &kLauncherApps[5],
};

const AppDescriptor *kLauncherPage2[8] = {
    &kLauncherApps[6], &kLauncherApps[7], &kLauncherApps[8],
    &kLauncherApps[9], &kLauncherApps[10], &kLauncherApps[11],
    &kLauncherApps[13],  /* Settings XML — declarative UI demo */
    &kLauncherApps[14],  /* Terminal — user XML schema demo */
};

const AppDescriptor *kLauncherDock[4] = {
    &kLauncherApps[0],  /* Phone */
    &kLauncherApps[2],  /* Gallery */
    &kLauncherApps[1],  /* Camera */
    &kLauncherApps[12], /* Settings */
};

void launcher_register_app_activities(launcher::ActivityRegistry &registry)
{
    for (const AppDescriptor &app : kLauncherApps) {
        if (!registry.registerActivity(app.id, app.create)) {
            std::printf("[LauncherApplication] duplicate Activity: %s\n", app.id);
        }
    }
}
