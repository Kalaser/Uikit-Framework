#include "UIViewController.h"
#include "UIKitPlatform.h"

void UIViewController_init(UIViewController *controller,
                           void (*destroy)(UIViewController *self))
{
    if (!controller) {
        return;
    }
    controller->view         = NULL;
    controller->title        = NULL;
    controller->user_data    = NULL;
    controller->state        = UIViewControllerState_None;

    controller->onCreate     = NULL;
    controller->onStart      = NULL;
    controller->onResume     = NULL;
    controller->onPause      = NULL;
    controller->onStop       = NULL;
    controller->onDestroy    = NULL;

    controller->load_view           = NULL;
    controller->view_will_appear    = NULL;
    controller->view_did_appear     = NULL;
    controller->view_will_disappear = NULL;
    controller->view_did_disappear  = NULL;
    controller->on_destroy          = NULL;

    controller->_destroy = destroy;
}

/* ── Title ────────────────────────────────────────────────────────────────── */

void UIViewController_set_title(UIViewController *controller, const char *title)
{
    if (!controller) {
        return;
    }
    controller->title = title;
}

const char *UIViewController_get_title(UIViewController *controller)
{
    return controller ? controller->title : NULL;
}

/* ── View access ──────────────────────────────────────────────────────────── */

UIView *UIViewController_get_view(UIViewController *controller)
{
    if (!controller) {
        return NULL;
    }
    if (!controller->view && controller->load_view) {
        controller->view = controller->load_view(controller);
    }
    if (!controller->view) {
        controller->view = UIView_create(NULL);
    }
    return controller->view;
}

void UIViewController_set_view(UIViewController *controller, UIView *view)
{
    if (!controller) {
        return;
    }
    controller->view = view;
}

/* ── Lifecycle triggers (state machine guarded, idempotent) ───────────────── */

void UIViewController_onCreate(UIViewController *controller)
{
    if (!controller || controller->state != UIViewControllerState_None) {
        return;
    }
    controller->state = UIViewControllerState_Created;

    /* Ensure the root view exists before the callback runs */
    UIViewController_get_view(controller);

    if (controller->onCreate) {
        controller->onCreate(controller);
    }
}

void UIViewController_onStart(UIViewController *controller)
{
    if (!controller) {
        return;
    }
    /* Forward from Created (first entry) or Stopped (re-entry) */
    if (controller->state != UIViewControllerState_Created &&
        controller->state != UIViewControllerState_Stopped) {
        return;
    }
    controller->state = UIViewControllerState_Started;

    if (controller->onStart) {
        controller->onStart(controller);
    }
    if (controller->view_will_appear) {   /* deprecated compat */
        controller->view_will_appear(controller);
    }
}

void UIViewController_onResume(UIViewController *controller)
{
    if (!controller) {
        return;
    }
    /* Forward from Started (first entry) or Paused (resume) */
    if (controller->state != UIViewControllerState_Started &&
        controller->state != UIViewControllerState_Paused) {
        return;
    }
    controller->state = UIViewControllerState_Resumed;

    if (controller->onResume) {
        controller->onResume(controller);
    }
    if (controller->view_did_appear) {    /* deprecated compat */
        controller->view_did_appear(controller);
    }
}

void UIViewController_onPause(UIViewController *controller)
{
    if (!controller || controller->state != UIViewControllerState_Resumed) {
        return;
    }
    controller->state = UIViewControllerState_Paused;

    if (controller->onPause) {
        controller->onPause(controller);
    }
    if (controller->view_will_disappear) {   /* deprecated compat */
        controller->view_will_disappear(controller);
    }
}

void UIViewController_onStop(UIViewController *controller)
{
    if (!controller || controller->state != UIViewControllerState_Paused) {
        return;
    }
    controller->state = UIViewControllerState_Stopped;

    if (controller->onStop) {
        controller->onStop(controller);
    }
    if (controller->view_did_disappear) {    /* deprecated compat */
        controller->view_did_disappear(controller);
    }
}

void UIViewController_onDestroy(UIViewController *controller)
{
    if (!controller || controller->state == UIViewControllerState_Destroyed) {
        return;
    }
    bool was_none = (controller->state == UIViewControllerState_None);
    controller->state = UIViewControllerState_Destroyed;

    /* A controller destroyed before onCreate fires no callbacks */
    if (!was_none && controller->onDestroy) {
        controller->onDestroy(controller);
    }
    if (controller->on_destroy) {            /* deprecated compat */
        controller->on_destroy(controller);
    }
}

/* ── Teardown ─────────────────────────────────────────────────────────────── */

void UIViewController_deinit(UIViewController *controller)
{
    if (!controller) {
        return;
    }
    UIViewController_onDestroy(controller);   /* idempotent */

    if (controller->view) {
        UIView_destroy(controller->view);
        controller->view = NULL;
    }
}

void UIViewController_destroy(UIViewController *controller)
{
    if (!controller) {
        return;
    }
    UIViewController_deinit(controller);

    if (controller->_destroy) {
        controller->_destroy(controller);
    } else {
        UIKit_free(controller);
    }
}

/* ── Deprecated triggers (compat aliases) ─────────────────────────────────── */

void UIViewController_view_will_appear(UIViewController *controller)
{
    UIViewController_onStart(controller);
}

void UIViewController_view_did_appear(UIViewController *controller)
{
    UIViewController_onResume(controller);
}

void UIViewController_view_will_disappear(UIViewController *controller)
{
    UIViewController_onPause(controller);
}

void UIViewController_view_did_disappear(UIViewController *controller)
{
    UIViewController_onStop(controller);
}
