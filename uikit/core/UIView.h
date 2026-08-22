#ifndef UIKIT_VIEW_H
#define UIKIT_VIEW_H

#include "lvgl.h"
#include "UIEvent.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @file UIView.h
 * @brief The base view class — a thin wrapper around lv_obj_t.
 *
 * UIView is NOT a widget library. Every UIKit component maps one-to-one
 * onto an LVGL primitive. UIView provides:
 *   - Frame / Bounds
 *   - Position
 *   - Size
 *   - Visibility
 *   - User Interaction
 *   - Child Views
 *   - Event Handling
 *   - Appearance (background, border, corner radius, opacity)
 */

typedef struct UIView UIView;
typedef struct UIViewCallback UIViewCallback;

struct UIViewCallback {
    UIEventType      type;
    UIEventCallback  callback;
    void            *user_data;
    UIViewCallback  *next;
};

struct UIView {
    lv_obj_t        *lv_obj;
    UIView          *superview;
    UIViewCallback  *callbacks;
    void            *tag;
    uint32_t         registered_events;   /**< bitmask of UIEventType already wired to LVGL */
};

/* ── Lifecycle ────────────────────────────────────────────────────────────── */

/**
 * Create a new UIView attached to parent (or the active screen if NULL).
 * @return The new view, or NULL on failure.
 */
UIView *UIView_create(UIView *parent);

/**
 * Destroy a view and free all resources, including registered callbacks.
 */
void UIView_destroy(UIView *view);

/* ── Frame / Layout ───────────────────────────────────────────────────────── */

/**
 * Set the view's position and size.
 */
void UIView_set_frame(UIView *view, int32_t x, int32_t y, int32_t width, int32_t height);

/**
 * Get the view's position and size.
 * Any output pointer may be NULL to skip that component.
 */
void UIView_get_frame(UIView *view, int32_t *x, int32_t *y, int32_t *width, int32_t *height);

/**
 * Set the view's position only.
 */
void UIView_set_position(UIView *view, int32_t x, int32_t y);

/**
 * Set the view's size only.
 */
void UIView_set_size(UIView *view, int32_t width, int32_t height);

/* ── Visibility ───────────────────────────────────────────────────────────── */

/**
 * Show or hide the view.
 */
void UIView_set_hidden(UIView *view, bool hidden);

/**
 * Check if the view is hidden.
 */
bool UIView_is_hidden(UIView *view);

/* ── View Hierarchy ───────────────────────────────────────────────────────── */

/**
 * Add child as a subview of parent.
 */
void UIView_add_subview(UIView *parent, UIView *child);

/**
 * Remove the view from its superview.
 */
void UIView_remove_from_superview(UIView *view);

/**
 * Remove `child` from `parent` (equivalent to UIView_remove_from_superview
 * on the child; the child object itself is kept alive).
 */
void UIView_remove_subview(UIView *parent, UIView *child);

/**
 * Get the view's superview, or NULL if it is a root view.
 */
UIView *UIView_get_superview(UIView *view);

/* ── Appearance ───────────────────────────────────────────────────────────── */

/**
 * Set the background color (0xRRGGBB format).
 */
void UIView_set_background_color(UIView *view, uint32_t color);

/**
 * Set the border color (0xRRGGBB) and width (pixels).
 */
void UIView_set_border(UIView *view, uint32_t color, int32_t width);

/**
 * Set the corner radius (pixels).
 */
void UIView_set_corner_radius(UIView *view, int32_t radius);

/**
 * Set the overall opacity (0 = fully transparent, 255 = fully opaque).
 */
void UIView_set_opacity(UIView *view, uint8_t opacity);

/* ── Interaction ──────────────────────────────────────────────────────────── */

/**
 * Enable or disable user interaction (touch, click, etc.).
 */
void UIView_set_user_interaction(UIView *view, bool enabled);

/**
 * Check if user interaction is enabled.
 */
bool UIView_is_user_interaction(UIView *view);

/**
 * Enable or disable the view (disabled views are greyed out and non-interactive).
 */
void UIView_set_enabled(UIView *view, bool enabled);

/**
 * Check if the view is enabled.
 */
bool UIView_is_enabled(UIView *view);

/* ── Tag / User Data ──────────────────────────────────────────────────────── */

/**
 * Attach an arbitrary user pointer to the view.
 */
void UIView_set_tag(UIView *view, void *tag);

/**
 * Retrieve the user pointer previously set with UIView_set_tag.
 */
void *UIView_get_tag(UIView *view);

/* ── Events ───────────────────────────────────────────────────────────────── */

/**
 * Register a callback for a specific event type.
 * @param view       The view to observe.
 * @param type       The event type (e.g. UIEVENT_CLICK).
 * @param callback   The callback function.
 * @param user_data  User-provided context (passed back in the UIEvent).
 */
void UIView_add_callback(UIView *view, UIEventType type, UIEventCallback callback, void *user_data);

/**
 * Remove a previously registered callback.
 */
void UIView_remove_callback(UIView *view, UIEventType type, UIEventCallback callback);

/* ── Native Access ────────────────────────────────────────────────────────── */

/**
 * Get the underlying LVGL object.
 * Use sparingly — prefer the UIKit API.
 */
lv_obj_t *UIView_native(UIView *view);

#endif /* UIKIT_VIEW_H */
