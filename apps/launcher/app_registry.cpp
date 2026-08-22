/**
 * @file app_registry.cpp
 * @brief Aggregates every standalone app into the launcher tables.
 */

#include "app_factory.h"

const AppMeta kLauncherPage1[6] = {
    { "Phone",   LV_SYMBOL_CALL,      0x34D399, phone_app_create },
    { "Camera",  LV_SYMBOL_EYE_OPEN,  0x60A5FA, camera_app_create },
    { "Gallery", LV_SYMBOL_IMAGE,     0xF472B6, gallery_app_create },
    { "Music",   LV_SYMBOL_AUDIO,     0xA78BFA, music_app_create },
    { "Weather", LV_SYMBOL_TINT,      0x38BDF8, weather_app_create },
    { "Maps",    LV_SYMBOL_GPS,       0x4ADE80, maps_app_create },
};

const AppMeta kLauncherPage2[6] = {
    { "Clock",   LV_SYMBOL_REFRESH,   0xFBBF24, clock_app_create },
    { "Calc",    LV_SYMBOL_EDIT,      0x2DD4BF, calc_app_create },
    { "Files",   LV_SYMBOL_DIRECTORY, 0xFB923C, files_app_create },
    { "Mail",    LV_SYMBOL_ENVELOPE,  0x818CF8, mail_app_create },
    { "Store",   LV_SYMBOL_DOWNLOAD,  0xF87171, store_app_create },
    { "Video",   LV_SYMBOL_VIDEO,     0xE879F9, video_app_create },
};

const AppMeta kLauncherDock[4] = {
    { "Phone",    LV_SYMBOL_CALL,      0x34D399, phone_app_create },
    { "Gallery",  LV_SYMBOL_IMAGE,     0xF472B6, gallery_app_create },
    { "Camera",   LV_SYMBOL_EYE_OPEN,  0x60A5FA, camera_app_create },
    { "Settings", LV_SYMBOL_SETTINGS,  0x334155, settings_app_create },
};
