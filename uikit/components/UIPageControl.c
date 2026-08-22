#include "UIPageControl.h"
#include "UIKitPlatform.h"

#define UIKIT_PC_DOT_SIZE      8
#define UIKIT_PC_DOT_SPACING   8
#define UIKIT_PC_HEIGHT        12

/* Default colors, overridable via setters */
static uint32_t s_current_color = 0x007AFF; /* UIKit blue */
static uint32_t s_page_color    = 0xC7C7CC; /* light grey */

static void rebuild_dots(UIPageControl *control)
{
    if (!control || !control->base.lv_obj) {
        return;
    }

    /* Remove existing dots */
    lv_obj_clean(control->base.lv_obj);

    for (uint32_t i = 0; i < control->page_count; i++) {
        lv_obj_t *dot = lv_obj_create(control->base.lv_obj);
        if (!dot) {
            continue;
        }
        lv_obj_set_size(dot, UIKIT_PC_DOT_SIZE, UIKIT_PC_DOT_SIZE);
        lv_obj_set_pos(dot, (int32_t)i * (UIKIT_PC_DOT_SIZE + UIKIT_PC_DOT_SPACING), 2);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_border_width(dot, 0, 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
        lv_obj_set_style_pad_all(dot, 0, 0);

        if (i == control->current) {
            lv_obj_set_style_bg_color(dot, lv_color_hex(s_current_color), 0);
            lv_obj_set_size(dot, UIKIT_PC_DOT_SIZE + 4, UIKIT_PC_DOT_SIZE);
        } else {
            lv_obj_set_style_bg_color(dot, lv_color_hex(s_page_color), 0);
        }
    }
}

UIPageControl *UIPageControl_create(UIView *parent)
{
    UIPageControl *control = UIKit_calloc(1, sizeof(UIPageControl));
    if (!control) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    control->base.lv_obj = lv_obj_create(parent_obj);
    if (!control->base.lv_obj) {
        UIKit_free(control);
        return NULL;
    }

    control->base.superview = parent;
    control->base.callbacks = NULL;
    control->base.tag       = NULL;

    control->page_count = 1;
    control->current    = 0;

    /* Transparent container holding only dots */
    lv_obj_set_size(control->base.lv_obj,
                    UIKIT_PC_DOT_SIZE + UIKIT_PC_DOT_SPACING, UIKIT_PC_HEIGHT);
    lv_obj_set_style_bg_opa(control->base.lv_obj, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(control->base.lv_obj, 0, 0);
    lv_obj_set_style_pad_all(control->base.lv_obj, 0, 0);
    lv_obj_clear_flag(control->base.lv_obj, LV_OBJ_FLAG_SCROLLABLE);

    rebuild_dots(control);

    return control;
}

void UIPageControl_destroy(UIPageControl *control)
{
    if (!control) {
        return;
    }
    UIView_destroy(&control->base);
}

void UIPageControl_set_page_count(UIPageControl *control, uint32_t count)
{
    if (!control || count == 0) {
        return;
    }
    if (control->page_count == count) {
        return;
    }
    control->page_count = count;
    if (control->current >= count) {
        control->current = count - 1;
    }
    rebuild_dots(control);
}

uint32_t UIPageControl_get_page_count(UIPageControl *control)
{
    return control ? control->page_count : 0;
}

void UIPageControl_set_current_page(UIPageControl *control, uint32_t page)
{
    if (!control || page >= control->page_count) {
        return;
    }
    if (control->current == page) {
        return;
    }
    control->current = page;
    rebuild_dots(control);
}

uint32_t UIPageControl_get_current_page(UIPageControl *control)
{
    return control ? control->current : 0;
}

void UIPageControl_set_current_color(UIPageControl *control, uint32_t color)
{
    if (!control) {
        return;
    }
    s_current_color = color;
    rebuild_dots(control);
}

void UIPageControl_set_page_color(UIPageControl *control, uint32_t color)
{
    if (!control) {
        return;
    }
    s_page_color = color;
    rebuild_dots(control);
}

void UIPageControl_on_change(UIPageControl *control, UIEventCallback callback, void *user_data)
{
    if (!control) {
        return;
    }
    UIView_add_callback(&control->base, UIEVENT_VALUE_CHANGED, callback, user_data);
}
