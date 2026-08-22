#include "UIScrollView.h"
#include "UIKitPlatform.h"

UIScrollView *UIScrollView_create(UIView *parent)
{
    UIScrollView *view = UIKit_calloc(1, sizeof(UIScrollView));
    if (!view) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();

    view->base.lv_obj = lv_obj_create(parent_obj);
    if (!view->base.lv_obj) {
        UIKit_free(view);
        return NULL;
    }

    view->base.superview = parent;
    view->base.callbacks = NULL;
    view->base.tag       = NULL;

    /* Enable scrolling in both directions */
    lv_obj_set_scroll_dir(view->base.lv_obj, LV_DIR_ALL);
    lv_obj_set_scrollbar_mode(view->base.lv_obj, LV_SCROLLBAR_MODE_AUTO);

    return view;
}

void UIScrollView_destroy(UIScrollView *view)
{
    if (!view) {
        return;
    }
    UIView_destroy(&view->base);
}

void UIScrollView_set_vertical_scroll(UIScrollView *view, bool enabled)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_dir_t dir = lv_obj_get_scroll_dir(view->base.lv_obj);
    if (enabled) {
        dir |= LV_DIR_TOP | LV_DIR_BOTTOM;
    } else {
        dir &= ~(lv_dir_t)(LV_DIR_TOP | LV_DIR_BOTTOM);
    }
    lv_obj_set_scroll_dir(view->base.lv_obj, dir);
}

void UIScrollView_set_horizontal_scroll(UIScrollView *view, bool enabled)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_dir_t dir = lv_obj_get_scroll_dir(view->base.lv_obj);
    if (enabled) {
        dir |= LV_DIR_LEFT | LV_DIR_RIGHT;
    } else {
        dir &= ~(lv_dir_t)(LV_DIR_LEFT | LV_DIR_RIGHT);
    }
    lv_obj_set_scroll_dir(view->base.lv_obj, dir);
}

void UIScrollView_set_scrollbar_visible(UIScrollView *view, bool visible)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_obj_set_scrollbar_mode(view->base.lv_obj,
                              visible ? LV_SCROLLBAR_MODE_AUTO
                                      : LV_SCROLLBAR_MODE_OFF);
}

void UIScrollView_scroll_to_view(UIScrollView *view, UIView *subview)
{
    if (!view || !view->base.lv_obj || !subview || !subview->lv_obj) {
        return;
    }
    lv_obj_scroll_to_view(subview->lv_obj, LV_ANIM_ON);
}

void UIScrollView_scroll_to(UIScrollView *view, int32_t x, int32_t y)
{
    if (!view || !view->base.lv_obj) {
        return;
    }
    lv_obj_scroll_to(view->base.lv_obj, x, y, LV_ANIM_ON);
}
