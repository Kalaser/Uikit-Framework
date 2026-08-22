#include "UIButton.h"
#include "UIKitPlatform.h"

UIButton *UIButton_create(UIView *parent)
{
    UIButton *button = UIKit_calloc(1, sizeof(UIButton));
    if (!button) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    button->base.lv_obj = lv_btn_create(parent_obj);
    if (!button->base.lv_obj) {
        UIKit_free(button);
        return NULL;
    }

    button->base.superview = parent;
    button->base.callbacks = NULL;
    button->base.tag       = NULL;

    /* Create the child label for the title */
    button->label = lv_label_create(button->base.lv_obj);
    if (button->label) {
        lv_label_set_text(button->label, "");
        lv_obj_center(button->label);
    }

    return button;
}

void UIButton_destroy(UIButton *button)
{
    if (!button) {
        return;
    }
    /* UIView_destroy deletes the LVGL button, which cascades to the label */
    UIView_destroy(&button->base);
}

void UIButton_set_title(UIButton *button, const char *title)
{
    if (!button || !button->label || !title) {
        return;
    }
    lv_label_set_text(button->label, title);
    lv_obj_center(button->label);
}

void UIButton_set_text(UIButton *button, const char *text)
{
    UIButton_set_title(button, text);
}

const char *UIButton_get_title(UIButton *button)
{
    if (!button || !button->label) {
        return NULL;
    }
    return lv_label_get_text(button->label);
}

const char *UIButton_get_text(UIButton *button)
{
    return UIButton_get_title(button);
}

void UIButton_set_title_color(UIButton *button, uint32_t color)
{
    if (!button || !button->label) {
        return;
    }
    lv_obj_set_style_text_color(button->label, lv_color_hex(color), 0);
}

void UIButton_set_background_color(UIButton *button, uint32_t color)
{
    if (!button || !button->base.lv_obj) {
        return;
    }
    lv_obj_set_style_bg_color(button->base.lv_obj, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(button->base.lv_obj, LV_OPA_COVER, 0);
}

void UIButton_on_click(UIButton *button, UIEventCallback callback, void *user_data)
{
    if (!button) {
        return;
    }
    UIView_add_callback(&button->base, UIEVENT_CLICK, callback, user_data);
}

void UIButton_set_action(UIButton *button, UIEventCallback callback, void *user_data)
{
    UIButton_on_click(button, callback, user_data);
}

void UIButton_set_event_callback(UIButton *button, UIEventType type,
                                 UIEventCallback callback, void *user_data)
{
    if (!button) {
        return;
    }
    UIView_add_callback(&button->base, type, callback, user_data);
}
