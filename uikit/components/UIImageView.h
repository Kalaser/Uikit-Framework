#ifndef UIKIT_IMAGEVIEW_H
#define UIKIT_IMAGEVIEW_H

#include "../core/UIView.h"

/**
 * @file UIImageView.h
 * @brief An image component wrapping lv_img.
 */

typedef struct UIImageView UIImageView;

struct UIImageView {
    UIView base;   /**< Inherits from UIView */
};

/**
 * Create a new UIImageView attached to parent (or the active screen if NULL).
 */
UIImageView *UIImageView_create(UIView *parent);

/**
 * Destroy an image view and free its resources.
 */
void UIImageView_destroy(UIImageView *view);

/**
 * Set the image source.
 * @param src  Pointer to an lv_img_dsc_t, a path string (with file
 *             system support), or a symbol string (LV_SYMBOL_*).
 */
void UIImageView_set_image(UIImageView *view, const void *src);

/**
 * Set the image scale (256 = 100%, 128 = 50%, 512 = 200%).
 */
void UIImageView_set_scale(UIImageView *view, uint16_t scale);

/**
 * Set the rotation angle in 0.1 degrees (e.g. 450 = 45.0°).
 */
void UIImageView_set_rotation(UIImageView *view, int16_t angle);

/**
 * Enable anti-aliasing when rotating / scaling.
 */
void UIImageView_set_antialias(UIImageView *view, bool antialias);

#endif /* UIKIT_IMAGEVIEW_H */
