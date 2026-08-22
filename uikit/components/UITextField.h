#ifndef UIKIT_TEXTFIELD_H
#define UIKIT_TEXTFIELD_H

#include "../core/UIView.h"

/**
 * @file UITextField.h
 * @brief A single-line text input component wrapping lv_textarea
 *        (in one-line mode).
 */

typedef struct UITextField UITextField;

struct UITextField {
    UIView base;   /**< Inherits from UIView */
};

/**
 * Create a new single-line text field attached to parent
 * (or the active screen if NULL).
 */
UITextField *UITextField_create(UIView *parent);

/**
 * Destroy a text field and free its resources.
 */
void UITextField_destroy(UITextField *field);

/**
 * Set the text content.
 */
void UITextField_set_text(UITextField *field, const char *text);

/**
 * Get the text content (read-only, owned by the field).
 */
const char *UITextField_get_text(UITextField *field);

/**
 * Set the placeholder text shown when the field is empty.
 */
void UITextField_set_placeholder(UITextField *field, const char *text);

/**
 * Set the text color (0xRRGGBB).
 */
void UITextField_set_text_color(UITextField *field, uint32_t color);

/**
 * Set the maximum length in characters (0 = unlimited).
 */
void UITextField_set_max_length(UITextField *field, uint32_t length);

/**
 * Set the text as a password (characters shown as dots).
 */
void UITextField_set_password_mode(UITextField *field, bool password);

/**
 * Set whether the field is editable.
 */
void UITextField_set_editable(UITextField *field, bool editable);

/**
 * Register a callback fired when the text content changes.
 */
void UITextField_on_change(UITextField *field, UIEventCallback callback, void *user_data);

#endif /* UIKIT_TEXTFIELD_H */
