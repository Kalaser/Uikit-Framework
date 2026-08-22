#include "UILabel.h"
#include "UIKitPlatform.h"

UILabel *UILabel_create(UIView *parent)
{
    UILabel *label = UIKit_calloc(1, sizeof(UILabel));
    if (!label) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    label->base.lv_obj = lv_label_create(parent_obj);
    if (!label->base.lv_obj) {
        UIKit_free(label);
        return NULL;
    }

    label->base.superview = parent;
    label->base.callbacks = NULL;
    label->base.tag       = NULL;

    return label;
}

void UILabel_destroy(UILabel *label)
{
    if (!label) {
        return;
    }
    /* UIView_destroy handles callback cleanup and LVGL object deletion */
    UIView_destroy(&label->base);
}

void UILabel_set_text(UILabel *label, const char *text)
{
    if (!label || !label->base.lv_obj || !text) {
        return;
    }
    lv_label_set_text(label->base.lv_obj, text);
}

const char *UILabel_get_text(UILabel *label)
{
    if (!label || !label->base.lv_obj) {
        return NULL;
    }
    return lv_label_get_text(label->base.lv_obj);
}

void UILabel_set_color(UILabel *label, uint32_t color)
{
    if (!label || !label->base.lv_obj) {
        return;
    }
    lv_obj_set_style_text_color(label->base.lv_obj, lv_color_hex(color), 0);
}

void UILabel_set_alignment(UILabel *label, int32_t align)
{
    if (!label || !label->base.lv_obj) {
        return;
    }
    lv_text_align_t lv_align;
    switch (align) {
    case 0:  lv_align = LV_TEXT_ALIGN_LEFT;   break;
    case 1:  lv_align = LV_TEXT_ALIGN_CENTER;  break;
    case 2:  lv_align = LV_TEXT_ALIGN_RIGHT;   break;
    default: lv_align = LV_TEXT_ALIGN_LEFT;    break;
    }
    lv_obj_set_style_text_align(label->base.lv_obj, lv_align, 0);
}

void UILabel_set_font(UILabel *label, const void *font)
{
    if (!label || !label->base.lv_obj || !font) {
        return;
    }
    lv_obj_set_style_text_font(label->base.lv_obj, (const lv_font_t *)font, 0);
}
