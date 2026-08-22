#include "UISlider.h"
#include "UIKitPlatform.h"

UISlider *UISlider_create(UIView *parent)
{
    UISlider *slider = UIKit_calloc(1, sizeof(UISlider));
    if (!slider) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    slider->base.lv_obj = lv_slider_create(parent_obj);
    if (!slider->base.lv_obj) {
        UIKit_free(slider);
        return NULL;
    }

    slider->base.superview = parent;
    slider->base.callbacks = NULL;
    slider->base.tag       = NULL;

    return slider;
}

void UISlider_destroy(UISlider *slider)
{
    if (!slider) {
        return;
    }
    UIView_destroy(&slider->base);
}

void UISlider_set_range(UISlider *slider, int32_t min, int32_t max)
{
    if (!slider || !slider->base.lv_obj || min >= max) {
        return;
    }
    lv_slider_set_range(slider->base.lv_obj, min, max);
}

void UISlider_set_value(UISlider *slider, int32_t value)
{
    if (!slider || !slider->base.lv_obj) {
        return;
    }
    lv_slider_set_value(slider->base.lv_obj, value, LV_ANIM_ON);
}

int32_t UISlider_get_value(UISlider *slider)
{
    if (!slider || !slider->base.lv_obj) {
        return 0;
    }
    return lv_slider_get_value(slider->base.lv_obj);
}

void UISlider_set_vertical(UISlider *slider, bool vertical)
{
    if (!slider || !slider->base.lv_obj) {
        return;
    }
    if (vertical) {
        lv_obj_set_height(slider->base.lv_obj, 150);
        lv_obj_set_width(slider->base.lv_obj, 8);
    } else {
        lv_obj_set_width(slider->base.lv_obj, 150);
        lv_obj_set_height(slider->base.lv_obj, 8);
    }
}

void UISlider_set_indicator_color(UISlider *slider, uint32_t color)
{
    if (!slider || !slider->base.lv_obj) {
        return;
    }
    lv_obj_set_style_bg_color(slider->base.lv_obj, lv_color_hex(color),
                              LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(slider->base.lv_obj, LV_OPA_COVER, LV_PART_INDICATOR);
}

void UISlider_set_knob_color(UISlider *slider, uint32_t color)
{
    if (!slider || !slider->base.lv_obj) {
        return;
    }
    lv_obj_set_style_bg_color(slider->base.lv_obj, lv_color_hex(color),
                              LV_PART_KNOB);
    lv_obj_set_style_bg_opa(slider->base.lv_obj, LV_OPA_COVER, LV_PART_KNOB);
}

void UISlider_on_change(UISlider *slider, UIEventCallback callback, void *user_data)
{
    if (!slider) {
        return;
    }
    UIView_add_callback(&slider->base, UIEVENT_VALUE_CHANGED, callback, user_data);
}
