#ifndef UIKIT_STACKVIEW_H
#define UIKIT_STACKVIEW_H

#include "../core/UIView.h"

/**
 * @file UIStackView.h
 * @brief A flex-based container that arranges its children in a row
 *        or a column, wrapping LVGL's flex layout.
 */

typedef enum {
    UIStackAxis_HORIZONTAL = 0,  /**< Children laid out left-to-right */
    UIStackAxis_VERTICAL   = 1   /**< Children laid out top-to-bottom */
} UIStackAxis;

typedef struct UIStackView UIStackView;

struct UIStackView {
    UIView base;   /**< Inherits from UIView */
};

/**
 * Create a new horizontal stack view attached to parent
 * (or the active screen if NULL).
 */
UIStackView *UIStackView_create(UIView *parent);

/**
 * Destroy a stack view and free its resources.
 */
void UIStackView_destroy(UIStackView *stack);

/**
 * Set the stacking axis (horizontal or vertical).
 */
void UIStackView_set_axis(UIStackView *stack, UIStackAxis axis);

/**
 * Set the spacing between arranged children (pixels).
 */
void UIStackView_set_spacing(UIStackView *stack, int32_t spacing);

/**
 * Set the padding inside the stack (pixels).
 */
void UIStackView_set_padding(UIStackView *stack, int32_t padding);

/**
 * Add a child to the stack. The child is reparented and arranged.
 */
void UIStackView_add_arranged_subview(UIStackView *stack, UIView *child);

/**
 * Set the alignment of children along the cross axis.
 * @param align  0 = start, 1 = center, 2 = stretch, 3 = end
 *               (LVGL 8 has no native stretch — 2 maps to start).
 */
void UIStackView_set_alignment(UIStackView *stack, int32_t align);

#endif /* UIKIT_STACKVIEW_H */
