#include "UISwitch.h"
#include "UIKitPlatform.h"

UISwitch *UISwitch_create(UIView *parent)
{
    UISwitch *sw = UIKit_calloc(1, sizeof(UISwitch));
    if (!sw) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    sw->base.lv_obj = lv_switch_create(parent_obj);
    if (!sw->base.lv_obj) {
        UIKit_free(sw);
        return NULL;
    }

    sw->base.superview = parent;
    sw->base.callbacks = NULL;
    sw->base.tag       = NULL;

    return sw;
}

void UISwitch_destroy(UISwitch *sw)
{
    if (!sw) {
        return;
    }
    UIView_destroy(&sw->base);
}

void UISwitch_set_on(UISwitch *sw, bool on)
{
    if (!sw || !sw->base.lv_obj) {
        return;
    }
    if (on) {
        lv_obj_add_state(sw->base.lv_obj, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(sw->base.lv_obj, LV_STATE_CHECKED);
    }
}

void UISwitch_set_on_instant(UISwitch *sw, bool on)
{
    if (!sw || !sw->base.lv_obj) {
        return;
    }
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, NULL);
    lv_anim_del(sw->base.lv_obj, NULL);
    UISwitch_set_on(sw, on);
}

bool UISwitch_is_on(UISwitch *sw)
{
    if (!sw || !sw->base.lv_obj) {
        return false;
    }
    return lv_obj_has_state(sw->base.lv_obj, LV_STATE_CHECKED);
}

void UISwitch_set_knob_color(UISwitch *sw, uint32_t color)
{
    if (!sw || !sw->base.lv_obj) {
        return;
    }
    lv_obj_set_style_bg_color(sw->base.lv_obj, lv_color_hex(color),
                              LV_PART_KNOB);
    lv_obj_set_style_bg_opa(sw->base.lv_obj, LV_OPA_COVER, LV_PART_KNOB);
}

void UISwitch_set_on_color(UISwitch *sw, uint32_t color)
{
    if (!sw || !sw->base.lv_obj) {
        return;
    }
    lv_obj_set_style_bg_color(sw->base.lv_obj, lv_color_hex(color),
                              LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(sw->base.lv_obj, LV_OPA_COVER,
                            LV_PART_INDICATOR | LV_STATE_CHECKED);
}

void UISwitch_on_change(UISwitch *sw, UIEventCallback callback, void *user_data)
{
    if (!sw) {
        return;
    }
    UIView_add_callback(&sw->base, UIEVENT_VALUE_CHANGED, callback, user_data);
}
