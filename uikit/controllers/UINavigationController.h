#ifndef UIKIT_NAVIGATIONCONTROLLER_H
#define UIKIT_NAVIGATIONCONTROLLER_H

#include "UIViewController.h"

/**
 * @file UINavigationController.h
 * @brief Stack-based navigation controller, like UIKit's UINavigationController.
 *
 * Push / pop view controllers with animated slide transitions.
 * The root controller is never popped (pop on a root-only stack is a no-op).
 */

typedef struct UINavigationController UINavigationController;

/**
 * Navigation stack depth limit to guard against runaway pushes.
 */
#define UIKIT_NAV_MAX_DEPTH 16

UINavigationController *UINavigationController_create(UIViewController *root);

/**
 * Destroy the navigation controller.
 * Destroys all controllers currently on the stack.
 * Does NOT destroy controllers already popped (caller owns them).
 */
void UINavigationController_destroy(UINavigationController *nav);

/**
 * Push a controller onto the stack with a slide-in animation.
 */
void UINavigationController_push(UINavigationController *nav, UIViewController *controller);

/**
 * Pop the top controller with a slide-out animation.
 * The popped controller is NOT destroyed — the caller owns it and may
 * push it again or destroy it.
 * @return The popped controller, or NULL if only the root remains.
 */
UIViewController *UINavigationController_pop(UINavigationController *nav);

/**
 * Pop everything except the root controller.
 * @return The number of controllers popped.
 */
int UINavigationController_pop_to_root(UINavigationController *nav);

/**
 * Get the number of controllers on the stack.
 */
int UINavigationController_count(UINavigationController *nav);

/**
 * Get the controller at a given stack index (0 = root), or NULL.
 */
UIViewController *UINavigationController_at(UINavigationController *nav, int index);

/**
 * Get the top (visible) controller, or NULL.
 */
UIViewController *UINavigationController_top(UINavigationController *nav);

/**
 * Enable or disable animated transitions.
 */
void UINavigationController_set_animated(UINavigationController *nav, bool animated);

/**
 * Resize the navigation surface (container + every stacked controller's view)
 * to a new screen size, e.g. when the window/display resolution changes.
 */
void UINavigationController_resize(UINavigationController *nav, int32_t width, int32_t height);

#endif /* UIKIT_NAVIGATIONCONTROLLER_H */
