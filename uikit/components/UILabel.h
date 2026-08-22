#ifndef UIKIT_LABEL_H
#define UIKIT_LABEL_H

#include "../core/UIView.h"

/**
 * @file UILabel.h
 * @brief A text label component wrapping lv_label.
 */

typedef struct UILabel UILabel;

struct UILabel {
    UIView base;   /**< Inherits from UIView */
};

/**
 * Create a new UILabel attached to parent (or the active screen if NULL).
 */
UILabel *UILabel_create(UIView *parent);

/**
 * Destroy a label and free its resources.
 */
void UILabel_destroy(UILabel *label);

/**
 * Set the label text.
 */
void UILabel_set_text(UILabel *label, const char *text);

/**
 * Get the label text (read-only, owned by the label).
 */
const char *UILabel_get_text(UILabel *label);

/**
 * Set the text color (0xRRGGBB).
 */
void UILabel_set_color(UILabel *label, uint32_t color);

/**
 * Set the text alignment.
 * @param align  0=left, 1=center, 2=right
 */
void UILabel_set_alignment(UILabel *label, int32_t align);

/**
 * Set the font.
 * @param font  Pointer to an lv_font_t (e.g. &lv_font_montserrat_14).
 */
void UILabel_set_font(UILabel *label, const void *font);

#endif /* UIKIT_LABEL_H */
