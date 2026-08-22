#include "UIView.h"
#include "UIKitPlatform.h"
#include <stdlib.h>

/* ── Internal: LVGL event → UIEvent translation ───────────────────────────── */

static UIEventType translate_lvgl_event(lv_event_code_t code)
{
    switch (code) {
    case LV_EVENT_PRESSED:        return UIEVENT_TOUCH_DOWN;
    case LV_EVENT_RELEASED:       return UIEVENT_TOUCH_UP;
    case LV_EVENT_SHORT_CLICKED:  return UIEVENT_CLICK;
    case LV_EVENT_LONG_PRESSED:   return UIEVENT_LONG_PRESS;
    case LV_EVENT_VALUE_CHANGED:  return UIEVENT_VALUE_CHANGED;
    case LV_EVENT_FOCUSED:        return UIEVENT_FOCUS;
    case LV_EVENT_DEFOCUSED:      return UIEVENT_DEFOCUS;
    case LV_EVENT_KEY:            return UIEVENT_KEY;
    default:                      return UIEVENT_CUSTOM;
    }
}

static void lvgl_event_handler(lv_event_t *lv_event)
{
    UIView *view = lv_event_get_user_data(lv_event);
    if (!view) {
        return;
    }

    UIEventType type = translate_lvgl_event(lv_event_get_code(lv_event));

    int32_t px = 0, py = 0;
    lv_indev_t *indev = lv_indev_get_act();
    if (indev) {
        lv_point_t pt;
        lv_indev_get_point(indev, &pt);
        px = pt.x;
        py = pt.y;
    }

    UIEvent ui_event = {
        .type      = type,
        .target    = view,
        .user_data = NULL,
        .x         = px,
        .y         = py,
        .value     = 0
    };

    /* Dispatch to all registered callbacks matching this event type */
    UIViewCallback *cb = view->callbacks;
    while (cb) {
        if (cb->type == type) {
            ui_event.user_data = cb->user_data;
            if (cb->callback) {
                cb->callback(&ui_event, cb->user_data);
            }
        }
        cb = cb->next;
    }
}

static void register_lvgl_event(UIView *view, UIEventType type)
{
    lv_event_code_t codes[] = {
        [UIEVENT_TOUCH_DOWN]    = LV_EVENT_PRESSED,
        [UIEVENT_TOUCH_UP]      = LV_EVENT_RELEASED,
        [UIEVENT_CLICK]         = LV_EVENT_SHORT_CLICKED,
        [UIEVENT_LONG_PRESS]    = LV_EVENT_LONG_PRESSED,
        [UIEVENT_VALUE_CHANGED] = LV_EVENT_VALUE_CHANGED,
        [UIEVENT_FOCUS]         = LV_EVENT_FOCUSED,
        [UIEVENT_DEFOCUS]       = LV_EVENT_DEFOCUSED,
        [UIEVENT_KEY]           = LV_EVENT_KEY,
    };

    if (type >= (int)(sizeof(codes) / sizeof(codes[0])) || codes[type] == 0) {
        return;
    }

    /* Register the LVGL event handler only once per event type */
    uint32_t mask = (uint32_t)1 << type;
    if (view->registered_events & mask) {
        return;
    }
    view->registered_events |= mask;

    lv_obj_add_event_cb(view->lv_obj, lvgl_event_handler, codes[type], view);
}

/* ── Lifecycle ────────────────────────────────────────────────────────────── */

UIView *UIView_create(UIView *parent)
{
    UIView *view = UIKit_calloc(1, sizeof(UIView));
    if (!view) {
        return NULL;
    }

    lv_obj_t *parent_obj = parent ? parent->lv_obj : lv_scr_act();
    if (!parent_obj) {
        UIKit_free(view);
        return NULL;
    }

    view->lv_obj = lv_obj_create(parent_obj);
    if (!view->lv_obj) {
        UIKit_free(view);
        return NULL;
    }

    view->superview  = parent;
    view->callbacks  = NULL;
    view->tag        = NULL;

    return view;
}

void UIView_destroy(UIView *view)
{
    if (!view) {
        return;
    }

    /* Free all registered callbacks */
    UIViewCallback *cb = view->callbacks;
    while (cb) {
        UIViewCallback *next = cb->next;
        UIKit_free(cb);
        cb = next;
    }
    view->callbacks = NULL;

    /* Delete the LVGL object (this also removes event handlers) */
    if (view->lv_obj) {
        lv_obj_del(view->lv_obj);
        view->lv_obj = NULL;
    }

    view->superview = NULL;
    view->tag = NULL;

    UIKit_free(view);
}

/* ── Frame / Layout ───────────────────────────────────────────────────────── */

void UIView_set_frame(UIView *view, int32_t x, int32_t y, int32_t width, int32_t height)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_pos(view->lv_obj, x, y);
    lv_obj_set_size(view->lv_obj, width, height);
}

void UIView_get_frame(UIView *view, int32_t *x, int32_t *y, int32_t *width, int32_t *height)
{
    if (!view || !view->lv_obj) {
        return;
    }
    /* Return the style-applied frame (what the caller set).
     * lv_obj_get_x/y/width/height read post-layout cached coords which are
     * stale until lv_timer_handler() runs — style getters are immediate. */
    if (x)      *x      = lv_obj_get_style_x(view->lv_obj, LV_PART_MAIN);
    if (y)      *y      = lv_obj_get_style_y(view->lv_obj, LV_PART_MAIN);
    if (width)  *width  = lv_obj_get_style_width(view->lv_obj, LV_PART_MAIN);
    if (height) *height = lv_obj_get_style_height(view->lv_obj, LV_PART_MAIN);
}

void UIView_set_position(UIView *view, int32_t x, int32_t y)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_pos(view->lv_obj, x, y);
}

void UIView_set_size(UIView *view, int32_t width, int32_t height)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_size(view->lv_obj, width, height);
}

/* ── Visibility ───────────────────────────────────────────────────────────── */

void UIView_set_hidden(UIView *view, bool hidden)
{
    if (!view || !view->lv_obj) {
        return;
    }
    if (hidden) {
        lv_obj_add_flag(view->lv_obj, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(view->lv_obj, LV_OBJ_FLAG_HIDDEN);
    }
}

bool UIView_is_hidden(UIView *view)
{
    if (!view || !view->lv_obj) {
        return true;
    }
    return lv_obj_has_flag(view->lv_obj, LV_OBJ_FLAG_HIDDEN);
}

/* ── View Hierarchy ───────────────────────────────────────────────────────── */

void UIView_add_subview(UIView *parent, UIView *child)
{
    if (!parent || !child || !parent->lv_obj || !child->lv_obj) {
        return;
    }
    lv_obj_set_parent(child->lv_obj, parent->lv_obj);
    child->superview = parent;
}

void UIView_remove_from_superview(UIView *view)
{
    if (!view || !view->lv_obj) {
        return;
    }
    /* Move to the active screen as a temporary parent */
    lv_obj_set_parent(view->lv_obj, lv_scr_act());
    view->superview = NULL;
}

void UIView_remove_subview(UIView *parent, UIView *child)
{
    (void)parent;
    UIView_remove_from_superview(child);
}

UIView *UIView_get_superview(UIView *view)
{
    if (!view) {
        return NULL;
    }
    return view->superview;
}

/* ── Appearance ───────────────────────────────────────────────────────────── */

void UIView_set_background_color(UIView *view, uint32_t color)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_style_bg_color(view->lv_obj, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(view->lv_obj, LV_OPA_COVER, 0);
}

void UIView_set_border(UIView *view, uint32_t color, int32_t width)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_style_border_color(view->lv_obj, lv_color_hex(color), 0);
    lv_obj_set_style_border_width(view->lv_obj, width, 0);
    lv_obj_set_style_border_opa(view->lv_obj, LV_OPA_COVER, 0);
}

void UIView_set_corner_radius(UIView *view, int32_t radius)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_style_radius(view->lv_obj, radius, 0);
}

void UIView_set_opacity(UIView *view, uint8_t opacity)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_style_opa(view->lv_obj, opacity, 0);
}

/* ── Interaction ──────────────────────────────────────────────────────────── */

void UIView_set_user_interaction(UIView *view, bool enabled)
{
    if (!view || !view->lv_obj) {
        return;
    }
    if (enabled) {
        lv_obj_add_flag(view->lv_obj, LV_OBJ_FLAG_CLICKABLE);
    } else {
        lv_obj_clear_flag(view->lv_obj, LV_OBJ_FLAG_CLICKABLE);
    }
}

bool UIView_is_user_interaction(UIView *view)
{
    if (!view || !view->lv_obj) {
        return false;
    }
    return lv_obj_has_flag(view->lv_obj, LV_OBJ_FLAG_CLICKABLE);
}

void UIView_set_enabled(UIView *view, bool enabled)
{
    if (!view || !view->lv_obj) {
        return;
    }
    if (enabled) {
        lv_obj_clear_state(view->lv_obj, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(view->lv_obj, LV_STATE_DISABLED);
    }
}

bool UIView_is_enabled(UIView *view)
{
    if (!view || !view->lv_obj) {
        return false;
    }
    return !lv_obj_has_state(view->lv_obj, LV_STATE_DISABLED);
}

/* ── Tag / User Data ──────────────────────────────────────────────────────── */

void UIView_set_tag(UIView *view, void *tag)
{
    if (!view) {
        return;
    }
    view->tag = tag;
}

void *UIView_get_tag(UIView *view)
{
    if (!view) {
        return NULL;
    }
    return view->tag;
}

/* ── Events ───────────────────────────────────────────────────────────────── */

void UIView_add_callback(UIView *view, UIEventType type, UIEventCallback callback, void *user_data)
{
    if (!view || !callback) {
        return;
    }

    UIViewCallback *cb = UIKit_malloc(sizeof(UIViewCallback));
    if (!cb) {
        return;
    }
    cb->type      = type;
    cb->callback  = callback;
    cb->user_data = user_data;
    cb->next      = view->callbacks;
    view->callbacks = cb;

    /* Register the corresponding LVGL event (deduplicated by LVGL) */
    register_lvgl_event(view, type);
}

void UIView_remove_callback(UIView *view, UIEventType type, UIEventCallback callback)
{
    if (!view || !callback) {
        return;
    }

    UIViewCallback **pp = &view->callbacks;
    while (*pp) {
        if ((*pp)->type == type && (*pp)->callback == callback) {
            UIViewCallback *to_free = *pp;
            *pp = to_free->next;
            UIKit_free(to_free);
            return;
        }
        pp = &(*pp)->next;
    }
}

/* ── Native Access ────────────────────────────────────────────────────────── */

lv_obj_t *UIView_native(UIView *view)
{
    return view ? view->lv_obj : NULL;
}
