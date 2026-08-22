#ifndef UIKIT_SCROLLVIEW_H
#define UIKIT_SCROLLVIEW_H

#include "../core/UIView.h"

/**
 * @file UIScrollView.h
 * @brief A scrollable container wrapping lv_obj with scrolling enabled
 *        (scroll flags are cleared on creation and re-enabled here).
 */

typedef struct UIScrollView UIScrollView;

struct UIScrollView {
    UIView base;   /**< Inherits from UIView */
};

/**
 * Create a new scrollable container attached to parent
 * (or the active screen if NULL).
 * Add subviews with UIView_add_subview — they will be scrollable.
 */
UIScrollView *UIScrollView_create(UIView *parent);

/**
 * Destroy a scroll view and free its resources.
 */
void UIScrollView_destroy(UIScrollView *view);

/**
 * Enable or disable vertical scrolling.
 */
void UIScrollView_set_vertical_scroll(UIScrollView *view, bool enabled);

/**
 * Enable or disable horizontal scrolling.
 */
void UIScrollView_set_horizontal_scroll(UIScrollView *view, bool enabled);

/**
 * Show or hide the scrollbar.
 */
void UIScrollView_set_scrollbar_visible(UIScrollView *view, bool visible);

/**
 * Scroll so that the given subview is fully visible.
 */
void UIScrollView_scroll_to_view(UIScrollView *view, UIView *subview);

/**
 * Scroll to a specific content offset (x, y) with animation.
 */
void UIScrollView_scroll_to(UIScrollView *view, int32_t x, int32_t y);

#endif /* UIKIT_SCROLLVIEW_H */
