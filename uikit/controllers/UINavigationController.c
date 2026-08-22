#include "UINavigationController.h"
#include "UIKitPlatform.h"
#include <string.h>

struct UINavigationController {
    UIViewController *stack[UIKIT_NAV_MAX_DEPTH];
    int               count;
    UIView           *container;   /**< Full-screen container on the active screen */
    bool              animated;
};

/* ── Animation helpers ─────────────────────────────────────────────────────── */

static int32_t screen_width(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    if (disp && disp->driver && disp->driver->hor_res > 0) {
        return disp->driver->hor_res;
    }
    return 320;
}

static int32_t screen_height(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    if (disp && disp->driver && disp->driver->ver_res > 0) {
        return disp->driver->ver_res;
    }
    return 240;
}

static void slide_in_anim(lv_obj_t *obj)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&a, screen_width(), 0);
    lv_anim_set_time(&a, 250);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
}

static void slide_out_anim(lv_obj_t *obj)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&a, lv_obj_get_x(obj), screen_width());
    lv_anim_set_time(&a, 250);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
    lv_anim_start(&a);
}

/* ── Internal ──────────────────────────────────────────────────────────────── */

static void attach_fullscreen(UIView *view)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_pos(view->lv_obj, 0, 0);
    lv_obj_set_size(view->lv_obj, screen_width(), screen_height());
}

/** attach_fullscreen with explicit dimensions (used on resize). */
static void attach_sized(UIView *view, int32_t w, int32_t h)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_pos(view->lv_obj, 0, 0);
    lv_obj_set_size(view->lv_obj, w, h);
}

/**
 * Detach a view from the navigation container onto the active screen.
 * Keeps the LVGL object alive so the owner can destroy it later.
 */
static void detach_to_screen(UIView *view)
{
    if (!view || !view->lv_obj) {
        return;
    }
    lv_obj_set_parent(view->lv_obj, lv_scr_act());
    lv_obj_set_pos(view->lv_obj, 0, 0);
    lv_obj_set_size(view->lv_obj, screen_width(), screen_height());
}

static void hide_view(UIView *view)
{
    if (view && view->lv_obj) {
        UIView_set_hidden(view, true);
    }
}

/* ── Public API ────────────────────────────────────────────────────────────── */

UINavigationController *UINavigationController_create(UIViewController *root)
{
    if (!root) {
        return NULL;
    }

    UINavigationController *nav = UIKit_calloc(1, sizeof(UINavigationController));
    if (!nav) {
        return NULL;
    }

    nav->count    = 0;
    nav->animated = true;

    nav->container = UIView_create(NULL);
    if (!nav->container) {
        UIKit_free(nav);
        return NULL;
    }
    attach_fullscreen(nav->container);
    lv_obj_clear_flag(nav->container->lv_obj, LV_OBJ_FLAG_SCROLLABLE);
    /* Container is transparent so the root view's background fills the
     * whole screen instead of being painted over by the container's default
     * white background. */
    lv_obj_set_style_bg_opa(nav->container->lv_obj, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(nav->container->lv_obj, 0, 0);

    /* Create + mount the root controller's view */
    UIView *view = UIViewController_get_view(root);
    if (!view) {
        UIView_destroy(nav->container);
        UIKit_free(nav);
        return NULL;
    }
    UIView_add_subview(nav->container, view);
    attach_fullscreen(view);

    nav->stack[nav->count++] = root;

    /* Lifecycle: create → start → resume (idempotent, state-guarded) */
    UIViewController_onCreate(root);
    UIViewController_onStart(root);
    UIViewController_onResume(root);

    return nav;
}

void UINavigationController_destroy(UINavigationController *nav)
{
    if (!nav) {
        return;
    }
    /* Tear down every stacked controller (pauses → stops → destroys),
     * BEFORE the container, so lv_obj_del on the container never frees
     * views still referenced by controllers. */
    for (int i = nav->count - 1; i >= 0; i--) {
        UIViewController *c = nav->stack[i];
        UIViewController_onPause(c);
        UIViewController_onStop(c);
        UIViewController_destroy(c);   /* triggers onDestroy */
    }
    nav->count = 0;

    if (nav->container) {
        UIView_destroy(nav->container);
        nav->container = NULL;
    }
    UIKit_free(nav);
}

void UINavigationController_push(UINavigationController *nav, UIViewController *controller)
{
    if (!nav || !controller || nav->count >= UIKIT_NAV_MAX_DEPTH) {
        return;
    }

    UIViewController *old_top = nav->stack[nav->count - 1];

    /* Old top loses focus: pause → stop (no-ops unless it was resumed) */
    UIViewController_onPause(old_top);
    UIViewController_onStop(old_top);

    UIView *new_view = UIViewController_get_view(controller);
    UIView *old_view = UIViewController_get_view(old_top);
    if (!new_view) {
        return;
    }

    /* Re-attach if the controller was popped earlier and lives on the screen */
    UIView_add_subview(nav->container, new_view);
    attach_fullscreen(new_view);

    if (nav->animated && old_view && old_view->lv_obj) {
        lv_obj_set_x(new_view->lv_obj, screen_width());
        slide_in_anim(new_view->lv_obj);
        slide_out_anim(old_view->lv_obj);
        hide_view(old_view);
    } else if (old_view) {
        hide_view(old_view);
    }

    nav->stack[nav->count++] = controller;

    /* New controller enters: create → start → resume */
    UIViewController_onCreate(controller);   /* no-op if already created */
    UIViewController_onStart(controller);
    UIViewController_onResume(controller);
}

UIViewController *UINavigationController_pop(UINavigationController *nav)
{
    if (!nav || nav->count <= 1) {
        return NULL;
    }

    UIViewController *top   = nav->stack[nav->count - 1];
    UIViewController *under = nav->stack[nav->count - 2];

    /* Top loses focus: pause → stop */
    UIViewController_onPause(top);
    UIViewController_onStop(top);

    UIView *top_view   = UIViewController_get_view(top);
    UIView *under_view = UIViewController_get_view(under);

    if (under_view) {
        UIView_set_hidden(under_view, false);
        if (under_view->lv_obj) {
            lv_obj_set_x(under_view->lv_obj, 0);
        }
    }

    if (nav->animated && top_view && top_view->lv_obj) {
        slide_out_anim(top_view->lv_obj);
    }

    /* Ownership of the popped controller returns to the caller; keep its
     * view alive by detaching it from the container. No onDestroy here. */
    detach_to_screen(top_view);

    nav->count--;

    /* Underlying controller becomes visible again: start → resume */
    UIViewController_onStart(under);
    UIViewController_onResume(under);

    return top;
}

int UINavigationController_pop_to_root(UINavigationController *nav)
{
    if (!nav || nav->count <= 1) {
        return 0;
    }

    UIViewController *root = nav->stack[0];
    UIView *root_view = UIViewController_get_view(root);

    int popped = nav->count - 1;
    for (int i = nav->count - 1; i >= 1; i--) {
        UIViewController *c = nav->stack[i];
        UIViewController_onPause(c);
        UIViewController_onStop(c);
        UIView *v = UIViewController_get_view(c);
        if (v) {
            detach_to_screen(v);
            hide_view(v);
            if (v->lv_obj) {
                lv_obj_set_x(v->lv_obj, 0);
            }
        }
    }

    nav->count = 1;

    if (root_view) {
        UIView_set_hidden(root_view, false);
        if (root_view->lv_obj) {
            lv_obj_set_x(root_view->lv_obj, 0);
        }
    }
    UIViewController_onStart(root);
    UIViewController_onResume(root);

    return popped;
}

int UINavigationController_count(UINavigationController *nav)
{
    return nav ? nav->count : 0;
}

UIViewController *UINavigationController_at(UINavigationController *nav, int index)
{
    if (!nav || index < 0 || index >= nav->count) {
        return NULL;
    }
    return nav->stack[index];
}

UIViewController *UINavigationController_top(UINavigationController *nav)
{
    if (!nav || nav->count == 0) {
        return NULL;
    }
    return nav->stack[nav->count - 1];
}

void UINavigationController_set_animated(UINavigationController *nav, bool animated)
{
    if (!nav) {
        return;
    }
    nav->animated = animated;
}

void UINavigationController_resize(UINavigationController *nav, int32_t width, int32_t height)
{
    if (!nav || !nav->container || width <= 0 || height <= 0) {
        return;
    }
    attach_sized(nav->container, width, height);
    for (int i = 0; i < nav->count; i++) {
        UIView *v = UIViewController_get_view(nav->stack[i]);
        if (v) {
            attach_sized(v, width, height);
        }
    }
}
