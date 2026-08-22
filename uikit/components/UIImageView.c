#include "UIImageView.h"
#include "UIKitPlatform.h"

UIImageView *UIImageView_create(UIView *parent)
{
    UIImageView *view = UIKit_calloc(1, sizeof(UIImageView));
    if (!view) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    view->base.lv_obj = lv_img_create(parent_obj);
    if (!view->base.lv_obj) {
        UIKit_free(view);
        return NULL;
    }

    view->base.superview = parent;
    view->base.callbacks = NULL;
    view->base.tag       = NULL;

    return view;
}

void UIImageView_destroy(UIImageView *view)
{
    if (!view) {
        return;
    }
    UIView_destroy(&view->base);
}

void UIImageView_set_image(UIImageView *view, const void *src)
{
    if (!view || !view->base.lv_obj || !src) {
        return;
    }
    lv_img_set_src(view->base.lv_obj, src);
}

void UIImageView_set_scale(UIImageView *view, uint16_t scale)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_img_set_zoom(view->base.lv_obj, scale);
}

void UIImageView_set_rotation(UIImageView *view, int16_t angle)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_img_set_angle(view->base.lv_obj, angle);
}

void UIImageView_set_antialias(UIImageView *view, bool antialias)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_img_set_antialias(view->base.lv_obj, antialias);
}
