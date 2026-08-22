#ifndef UIKIT_BUTTON_H
#define UIKIT_BUTTON_H

#include "../core/UIView.h"

/**
 * @file UIButton.h
 * @brief A button component wrapping lv_btn with a child label.
 */

typedef struct UIButton UIButton;

struct UIButton {
    UIView base;       /**< Inherits from UIView — base.lv_obj is the button object */
    lv_obj_t *label;   /**< Child label for the button title */
};

/**
 * Create a new UIButton attached to parent (or the active screen if NULL).
 */
UIButton *UIButton_create(UIView *parent);

/**
 * Destroy a button and free its resources.
 */
void UIButton_destroy(UIButton *button);

/**
 * Set the button title text.
 */
void UIButton_set_title(UIButton *button, const char *title);

/**
 * Alias of UIButton_set_title (README API convention).
 */
void UIButton_set_text(UIButton *button, const char *text);

/**
 * Get the button title text (read-only).
 */
const char *UIButton_get_title(UIButton *button);

/**
 * Get the button title text (read-only).
 */
const char *UIButton_get_text(UIButton *button);

/**
 * Set the title text color (0xRRGGBB).
 */
void UIButton_set_title_color(UIButton *button, uint32_t color);

/**
 * Set the button background color (0xRRGGBB).
 */
void UIButton_set_background_color(UIButton *button, uint32_t color);

/**
 * Register a click callback.
 * This is a convenience wrapper around UIView_add_callback.
 *
 * @param button     The button.
 * @param callback   The callback function.
 * @param user_data  User-provided context.
 */
void UIButton_on_click(UIButton *button, UIEventCallback callback, void *user_data);

/**
 * Alias of UIButton_on_click (README API convention).
 */
void UIButton_set_action(UIButton *button, UIEventCallback callback, void *user_data);

/**
 * Register a generic event callback for the button (any event type).
 */
void UIButton_set_event_callback(UIButton *button, UIEventType type,
                                 UIEventCallback callback, void *user_data);

#endif /* UIKIT_BUTTON_H */
