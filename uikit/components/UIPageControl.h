#ifndef UIKIT_PAGECONTROL_H
#define UIKIT_PAGECONTROL_H

#include "../core/UIView.h"

/**
 * @file UIPageControl.h
 * @brief A page indicator (dots) component built on lv_obj with dot children.
 */

typedef struct UIPageControl UIPageControl;

struct UIPageControl {
    UIView base;        /**< Inherits from UIView */
    uint32_t page_count; /**< Total number of pages */
    uint32_t current;    /**< Current page index */
};

/**
 * Create a new UIPageControl attached to parent (or the active screen if NULL).
 */
UIPageControl *UIPageControl_create(UIView *parent);

/**
 * Destroy a page control and free its resources.
 */
void UIPageControl_destroy(UIPageControl *control);

/**
 * Set the total number of pages (rebuilds the dots).
 */
void UIPageControl_set_page_count(UIPageControl *control, uint32_t count);

/**
 * Get the total number of pages.
 */
uint32_t UIPageControl_get_page_count(UIPageControl *control);

/**
 * Set the current page index (highlights that dot, 0-based).
 */
void UIPageControl_set_current_page(UIPageControl *control, uint32_t page);

/**
 * Get the current page index.
 */
uint32_t UIPageControl_get_current_page(UIPageControl *control);

/**
 * Set the dot color for the current page (0xRRGGBB).
 */
void UIPageControl_set_current_color(UIPageControl *control, uint32_t color);

/**
 * Set the dot color for other pages (0xRRGGBB).
 */
void UIPageControl_set_page_color(UIPageControl *control, uint32_t color);

/**
 * Register a callback fired when the current page changes
 * (event.value holds the new page index).
 */
void UIPageControl_on_change(UIPageControl *control, UIEventCallback callback, void *user_data);

#endif /* UIKIT_PAGECONTROL_H */
