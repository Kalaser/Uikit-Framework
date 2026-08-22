#include "UIProgressView.h"
#include "UIKitPlatform.h"

UIProgressView *UIProgressView_create(UIView *parent)
{
    UIProgressView *view = UIKit_calloc(1, sizeof(UIProgressView));
    if (!view) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    view->base.lv_obj = lv_bar_create(parent_obj);
    if (!view->base.lv_obj) {
        UIKit_free(view);
        return NULL;
    }

    view->base.superview = parent;
    view->base.callbacks = NULL;
    view->base.tag       = NULL;

    lv_bar_set_range(view->base.lv_obj, 0, 100);

    return view;
}

void UIProgressView_destroy(UIProgressView *view)
{
    if (!view) {
        return;
    }
    UIView_destroy(&view->base);
}

void UIProgressView_set_range(UIProgressView *view, int32_t min, int32_t max)
{
    if (!view || !view->base.lv_obj || min >= max) {
        return;
    }
    lv_bar_set_range(view->base.lv_obj, min, max);
}

void UIProgressView_set_value(UIProgressView *view, int32_t value)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_bar_set_value(view->base.lv_obj, value, LV_ANIM_ON);
}

void UIProgressView_set_value_instant(UIProgressView *view, int32_t value)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_bar_set_value(view->base.lv_obj, value, LV_ANIM_OFF);
}

int32_t UIProgressView_get_value(UIProgressView *view)
{
    if (!view || !view->base.lv_obj) {
        return 0;
    }
    return lv_bar_get_value(view->base.lv_obj);
}

void UIProgressView_set_track_color(UIProgressView *view, uint32_t color)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_obj_set_style_bg_color(view->base.lv_obj, lv_color_hex(color),
                              LV_PART_MAIN);
    lv_obj_set_style_bg_opa(view->base.lv_obj, LV_OPA_COVER, LV_PART_MAIN);
}

void UIProgressView_set_progress_color(UIProgressView *view, uint32_t color)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_obj_set_style_bg_color(view->base.lv_obj, lv_color_hex(color),
                              LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(view->base.lv_obj, LV_OPA_COVER, LV_PART_INDICATOR);
}

void UIProgressView_set_vertical(UIProgressView *view, bool vertical)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    if (vertical) {
        lv_obj_set_width(view->base.lv_obj, 8);
        lv_obj_set_height(view->base.lv_obj, 100);
    } else {
        lv_obj_set_width(view->base.lv_obj, 150);
        lv_obj_set_height(view->base.lv_obj, 8);
    }
}
