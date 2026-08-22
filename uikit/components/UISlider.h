#ifndef UIKIT_SLIDER_H
#define UIKIT_SLIDER_H

#include "../core/UIView.h"

/**
 * @file UISlider.h
 * @brief A slider component wrapping lv_slider.
 */

typedef struct UISlider UISlider;

struct UISlider {
    UIView base;   /**< Inherits from UIView */
};

/**
 * Create a new UISlider attached to parent (or the active screen if NULL).
 */
UISlider *UISlider_create(UIView *parent);

/**
 * Destroy a slider and free its resources.
 */
void UISlider_destroy(UISlider *slider);

/**
 * Set the slider range.
 */
void UISlider_set_range(UISlider *slider, int32_t min, int32_t max);

/**
 * Set the current value.
 */
void UISlider_set_value(UISlider *slider, int32_t value);

/**
 * Get the current value.
 */
int32_t UISlider_get_value(UISlider *slider);

/**
 * Make the slider horizontal (default) or vertical.
 */
void UISlider_set_vertical(UISlider *slider, bool vertical);

/**
 * Set the indicator (filled portion) color (0xRRGGBB).
 */
void UISlider_set_indicator_color(UISlider *slider, uint32_t color);

/**
 * Set the knob color (0xRRGGBB).
 */
void UISlider_set_knob_color(UISlider *slider, uint32_t color);

/**
 * Register a callback fired when the value changes (including drag).
 */
void UISlider_on_change(UISlider *slider, UIEventCallback callback, void *user_data);

#endif /* UIKIT_SLIDER_H */
