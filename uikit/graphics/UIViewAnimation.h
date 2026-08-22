#ifndef UIKIT_ANIMATION_H
#define UIKIT_ANIMATION_H

#include "../core/UIView.h"

/**
 * @file UIViewAnimation.h
 * @brief Framework-level animation abstraction over LVGL animations.
 *
 * Animations are optional at runtime — a view with no active animation
 * carries no overhead.
 */

typedef enum {
    UIANIM_PROP_X = 0,   /**< Horizontal position */
    UIANIM_PROP_Y,       /**< Vertical position */
    UIANIM_PROP_WIDTH,   /**< Width */
    UIANIM_PROP_HEIGHT,  /**< Height */
    UIANIM_PROP_OPACITY  /**< Opacity (0..255) */
} UIAnimationProperty;

/**
 * Animate a view property from its current value to `to` over `duration_ms`.
 * @param view       The view to animate.
 * @param property   The property to animate.
 * @param to         Target value.
 * @param duration_ms Animation duration in milliseconds.
 */
void UIView_animate(UIView *view, UIAnimationProperty property,
                    int32_t to, uint16_t duration_ms);

/**
 * Stop all running animations on the view.
 */
void UIView_stop_animations(UIView *view);

#endif /* UIKIT_ANIMATION_H */
