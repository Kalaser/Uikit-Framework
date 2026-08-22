#include "UITextField.h"
#include "UIKitPlatform.h"

UITextField *UITextField_create(UIView *parent)
{
    UITextField *field = UIKit_calloc(1, sizeof(UITextField));
    if (!field) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    field->base.lv_obj = lv_textarea_create(parent_obj);
    if (!field->base.lv_obj) {
        UIKit_free(field);
        return NULL;
    }

    field->base.superview = parent;
    field->base.callbacks = NULL;
    field->base.tag       = NULL;

    /* Single-line mode, like a UIKit UITextField */
    lv_textarea_set_one_line(field->base.lv_obj, true);

    return field;
}

void UITextField_destroy(UITextField *field)
{
    if (!field) {
        return;
    }
    UIView_destroy(&field->base);
}

void UITextField_set_text(UITextField *field, const char *text)
{
    if (!field || !field->base.lv_obj) {
        return;
    }
    lv_textarea_set_text(field->base.lv_obj, text ? text : "");
}

const char *UITextField_get_text(UITextField *field)
{
    if (!field || !field->base.lv_obj) {
        return NULL;
    }
    return lv_textarea_get_text(field->base.lv_obj);
}

void UITextField_set_placeholder(UITextField *field, const char *text)
{
    if (!field || !field->base.lv_obj || !text) {
        return;
    }
    lv_textarea_set_placeholder_text(field->base.lv_obj, text);
}

void UITextField_set_text_color(UITextField *field, uint32_t color)
{
    if (!field || !field->base.lv_obj) {
        return;
    }
    lv_obj_set_style_text_color(field->base.lv_obj, lv_color_hex(color), 0);
}

void UITextField_set_max_length(UITextField *field, uint32_t length)
{
    if (!field || !field->base.lv_obj) {
        return;
    }
    lv_textarea_set_max_length(field->base.lv_obj, length);
}

void UITextField_set_password_mode(UITextField *field, bool password)
{
    if (!field || !field->base.lv_obj) {
        return;
    }
    lv_textarea_set_password_mode(field->base.lv_obj, password);
}

void UITextField_set_editable(UITextField *field, bool editable)
{
    if (!field || !field->base.lv_obj) {
        return;
    }
    if (editable) {
        lv_obj_clear_state(field->base.lv_obj, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(field->base.lv_obj, LV_STATE_DISABLED);
    }
}

void UITextField_on_change(UITextField *field, UIEventCallback callback, void *user_data)
{
    if (!field) {
        return;
    }
    UIView_add_callback(&field->base, UIEVENT_VALUE_CHANGED, callback, user_data);
}
