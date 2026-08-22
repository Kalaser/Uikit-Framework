#include "UIViewAnimation.h"

/* ── Property executors (match lv_anim_exec_xcb_t) ────────────────────────── */

static void exec_x(void *obj, int32_t value)
{
    lv_obj_set_x((lv_obj_t *)obj, value);
}

static void exec_y(void *obj, int32_t value)
{
    lv_obj_set_y((lv_obj_t *)obj, value);
}

static void exec_width(void *obj, int32_t value)
{
    lv_obj_set_width((lv_obj_t *)obj, value);
}

static void exec_height(void *obj, int32_t value)
{
    lv_obj_set_height((lv_obj_t *)obj, value);
}

static void exec_opacity(void *obj, int32_t value)
{
    lv_obj_set_style_opa((lv_obj_t *)obj, (lv_opa_t)value, 0);
}

/* ── Public API ────────────────────────────────────────────────────────────── */

void UIView_animate(UIView *view, UIAnimationProperty property,
                    int32_t to, uint16_t duration_ms)
{
    if (!view || !view->lv_obj || duration_ms == 0) {
        return;
    }

    lv_anim_exec_xcb_t exec = NULL;
    int32_t from = 0;

    switch (property) {
    case UIANIM_PROP_X:       exec = exec_x;       from = lv_obj_get_x(view->lv_obj);      break;
    case UIANIM_PROP_Y:       exec = exec_y;       from = lv_obj_get_y(view->lv_obj);      break;
    case UIANIM_PROP_WIDTH:   exec = exec_width;   from = lv_obj_get_width(view->lv_obj);  break;
    case UIANIM_PROP_HEIGHT:  exec = exec_height;  from = lv_obj_get_height(view->lv_obj); break;
    case UIANIM_PROP_OPACITY: exec = exec_opacity; from = lv_obj_get_style_opa(view->lv_obj, 0); break;
    default:
        return;
    }

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, view->lv_obj);
    lv_anim_set_exec_cb(&a, exec);
    lv_anim_set_values(&a, from, to);
    lv_anim_set_time(&a, duration_ms);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
}

void UIView_stop_animations(UIView *view)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_anim_del(view->lv_obj, NULL);
}
