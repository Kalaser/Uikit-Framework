#ifndef UIKIT_PROGRESSVIEW_H
#define UIKIT_PROGRESSVIEW_H

#include "../core/UIView.h"

/**
 * @file UIProgressView.h
 * @brief A progress bar component wrapping lv_bar.
 */

typedef struct UIProgressView UIProgressView;

struct UIProgressView {
    UIView base;   /**< Inherits from UIView */
};

/**
 * Create a new UIProgressView attached to parent (or the active screen if NULL).
 */
UIProgressView *UIProgressView_create(UIView *parent);

/**
 * Destroy a progress view and free its resources.
 */
void UIProgressView_destroy(UIProgressView *view);

/**
 * Set the progress range (defaults to 0..100).
 */
void UIProgressView_set_range(UIProgressView *view, int32_t min, int32_t max);

/**
 * Set the current progress value (animated).
 */
void UIProgressView_set_value(UIProgressView *view, int32_t value);

/**
 * Set the current progress value without animation.
 */
void UIProgressView_set_value_instant(UIProgressView *view, int32_t value);

/**
 * Get the current progress value.
 */
int32_t UIProgressView_get_value(UIProgressView *view);

/**
 * Set the track (background) color (0xRRGGBB).
 */
void UIProgressView_set_track_color(UIProgressView *view, uint32_t color);

/**
 * Set the progress (filled) color (0xRRGGBB).
 */
void UIProgressView_set_progress_color(UIProgressView *view, uint32_t color);

/**
 * Make the progress view vertical instead of horizontal.
 */
void UIProgressView_set_vertical(UIProgressView *view, bool vertical);

#endif /* UIKIT_PROGRESSVIEW_H */
