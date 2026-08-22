#ifndef UIKIT_VIEWCONTROLLER_H
#define UIKIT_VIEWCONTROLLER_H

#include "../core/UIView.h"

/**
 * @file UIViewController.h
 * @brief Controller layer with Android-style lifecycle management.
 *
 * Lifecycle callbacks are driven by an internal state machine, so every
 * callback fires at most once and always in a legal order, regardless of
 * how many times the navigation controller triggers it:
 *
 *   None → Created → Started → Resumed ⇄ Paused → Stopped → Destroyed
 *
 *   onCreate   once, first entry into a navigation stack (view is created)
 *   onStart    visible (no focus)
 *   onResume   fully visible & interactive (current screen)
 *   onPause    losing focus (covered by a push)
 *   onStop     fully invisible
 *   onDestroy  once, on real teardown (UIViewController_destroy)
 *
 * Subclass by embedding UIViewController as the first member:
 *
 *   typedef struct {
 *       UIViewController base;
 *       UILabel *title_label;
 *   } MyController;
 *
 *   MyController *MyController_create(void) {
 *       MyController *c = UIKit_calloc(1, sizeof(MyController));
 *       UIViewController_init(&c->base, NULL);
 *       c->base.load_view = my_load_view;      // view creation (optional)
 *       c->base.onCreate  = my_on_create;      // Android-style callbacks
 *       c->base.onResume  = my_on_resume;
 *       ...
 *       return c;
 *   }
 */

/* ── Lifecycle state machine ──────────────────────────────────────────────── */

typedef enum {
    UIViewControllerState_None = 0,    /**< Not yet created */
    UIViewControllerState_Created,     /**< onCreate called, view loaded */
    UIViewControllerState_Started,     /**< onStart called, visible */
    UIViewControllerState_Resumed,     /**< onResume called, interactive */
    UIViewControllerState_Paused,      /**< onPause called, lost focus */
    UIViewControllerState_Stopped,     /**< onStop called, invisible */
    UIViewControllerState_Destroyed    /**< onDestroy called, terminal */
} UIViewControllerState;

typedef struct UIViewController UIViewController;

struct UIViewController {
    UIView *view;                 /**< Root view (created in onCreate) */
    const char *title;            /**< Static title string (navigation bar) */
    void   *user_data;            /**< Arbitrary context (C++ binding uses this) */
    UIViewControllerState state;  /**< Lifecycle state — managed by the framework */

    /* ── Android-style lifecycle callbacks (primary API) ── */
    void (*onCreate)(UIViewController *self);
    void (*onStart)(UIViewController *self);
    void (*onResume)(UIViewController *self);
    void (*onPause)(UIViewController *self);
    void (*onStop)(UIViewController *self);
    void (*onDestroy)(UIViewController *self);

    /* ── Deprecated UIKit-style hooks ──────────────────────────────
     * Kept for compatibility; the state machine forwards to them
     * automatically. Prefer the Android-style callbacks above.
     *   load_view            → invoked during onCreate
     *   view_will_appear     → forwarded from onStart
     *   view_did_appear      → forwarded from onResume
     *   view_will_disappear  → forwarded from onPause
     *   view_did_disappear   → forwarded from onStop
     *   on_destroy           → forwarded from onDestroy                      */
    UIView *(*load_view)(UIViewController *self);
    void (*view_will_appear)(UIViewController *self);
    void (*view_did_appear)(UIViewController *self);
    void (*view_will_disappear)(UIViewController *self);
    void (*view_did_disappear)(UIViewController *self);
    void (*on_destroy)(UIViewController *self);

    void (*_destroy)(UIViewController *self);   /**< Internal: frees subclass memory */
};

/**
 * Initialize a controller (call from your subclass constructor).
 * @param controller  The controller (typically embedded as `base`).
 * @param destroy     Destructor that frees the subclass allocation,
 *                    or NULL if only the base is used.
 */
void UIViewController_init(UIViewController *controller,
                           void (*destroy)(UIViewController *self));

/* ── Title ────────────────────────────────────────────────────────────────── */

/**
 * Set the controller title (static string; not copied).
 */
void UIViewController_set_title(UIViewController *controller, const char *title);

/**
 * Get the controller title, or NULL.
 */
const char *UIViewController_get_title(UIViewController *controller);

/* ── View access ──────────────────────────────────────────────────────────── */

/**
 * Get the root view, creating it on first access via load_view
 * (or a plain UIView if no load_view is set).
 */
UIView *UIViewController_get_view(UIViewController *controller);

/**
 * Manually set the root view (optional alternative to load_view).
 */
void UIViewController_set_view(UIViewController *controller, UIView *view);

/* ── Lifecycle triggers (driven by the navigation controller) ───────────────
 *
 * Each trigger is idempotent and guarded by the state machine: invalid
 * transitions are ignored, so callers may invoke them unconditionally.
 * ────────────────────────────────────────────────────────────────────────── */

void UIViewController_onCreate(UIViewController *controller);  /* None → Created */
void UIViewController_onStart(UIViewController *controller);   /* Created|Stopped → Started */
void UIViewController_onResume(UIViewController *controller);  /* Started|Paused → Resumed */
void UIViewController_onPause(UIViewController *controller);   /* Resumed → Paused */
void UIViewController_onStop(UIViewController *controller);    /* Paused → Stopped */
void UIViewController_onDestroy(UIViewController *controller); /* any non-terminal → Destroyed */

/* ── Teardown ─────────────────────────────────────────────────────────────── */

/**
 * Run lifecycle teardown (onDestroy + destroy the root view) but do NOT
 * free the controller struct. Safe to call multiple times (idempotent).
 * Use this from C++ wrappers that embed the struct by value.
 */
void UIViewController_deinit(UIViewController *controller);

/**
 * Deinit + free the controller struct (via the subclass destructor
 * registered in UIViewController_init, or UIKit_free).
 */
void UIViewController_destroy(UIViewController *controller);

/* ── Deprecated triggers (compat aliases) ─────────────────────────────────── */

void UIViewController_view_will_appear(UIViewController *controller);
void UIViewController_view_did_appear(UIViewController *controller);
void UIViewController_view_will_disappear(UIViewController *controller);
void UIViewController_view_did_disappear(UIViewController *controller);

#endif /* UIKIT_VIEWCONTROLLER_H */
