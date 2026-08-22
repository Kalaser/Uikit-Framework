#ifndef UIKIT_SWITCH_H
#define UIKIT_SWITCH_H

#include "../core/UIView.h"

/**
 * @file UISwitch.h
 * @brief A toggle switch component wrapping lv_switch.
 */

typedef struct UISwitch UISwitch;

struct UISwitch {
    UIView base;   /**< Inherits from UIView */
};

/**
 * Create a new UISwitch attached to parent (or the active screen if NULL).
 */
UISwitch *UISwitch_create(UIView *parent);

/**
 * Destroy a switch and free its resources.
 */
void UISwitch_destroy(UISwitch *sw);

/**
 * Turn the switch on or off (animated).
 */
void UISwitch_set_on(UISwitch *sw, bool on);

/**
 * Turn the switch on or off without animation.
 */
void UISwitch_set_on_instant(UISwitch *sw, bool on);

/**
 * Check if the switch is on.
 */
bool UISwitch_is_on(UISwitch *sw);

/**
 * Set the knob color (0xRRGGBB).
 */
void UISwitch_set_knob_color(UISwitch *sw, uint32_t color);

/**
 * Set the background color when the switch is on (0xRRGGBB).
 */
void UISwitch_set_on_color(UISwitch *sw, uint32_t color);

/**
 * Register a callback fired when the switch state changes.
 */
void UISwitch_on_change(UISwitch *sw, UIEventCallback callback, void *user_data);

#endif /* UIKIT_SWITCH_H */
