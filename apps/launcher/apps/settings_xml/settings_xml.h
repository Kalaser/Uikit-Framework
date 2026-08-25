
#ifndef UIKIT_XML_SETTINGS_H
#define UIKIT_XML_SETTINGS_H

#include "UIKit.h"

#ifdef __cplusplus
extern "C" {
#endif

/* id -> widget map for the settings page (controllers bind events via these) */
typedef struct {
    UIButton *back;
    UISwitch *confirm_destructive_commands;
    UISwitch *auto_reconnect;
    UISwitch *keep_screen_awake;
} SettingsXmlIds;

UIView *settings_xml_build(SettingsXmlIds *ids);

#ifdef __cplusplus
}
#endif

#endif /* UIKIT_XML_SETTINGS_H */
