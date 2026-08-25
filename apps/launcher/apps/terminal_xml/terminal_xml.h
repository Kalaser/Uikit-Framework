
#ifndef UIKIT_XML_TERMINAL_H
#define UIKIT_XML_TERMINAL_H

#include "UIKit.h"

#ifdef __cplusplus
extern "C" {
#endif

/* id -> widget map for the terminal page (controllers bind events via these) */
typedef struct {
    UIButton *back;
    UIView *terminal_card;
    UILabel *cursor;
    UIView *actConnect;
    UIView *actScan;
    UIView *actReboot;
    UIView *factory_glass__24;
    UIView *debug_board__07;
    UIButton *tab_terminal;
    UIButton *tab_devices;
    UIButton *tab_settings;
} TerminalXmlIds;

UIView *terminal_xml_build(TerminalXmlIds *ids);

#ifdef __cplusplus
}
#endif

#endif /* UIKIT_XML_TERMINAL_H */
