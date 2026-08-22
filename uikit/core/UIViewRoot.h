#ifndef UIKIT_ROOT_VIEW_H
#define UIKIT_ROOT_VIEW_H

#include "../core/UIView.h"

/**
 * Create the root UIView for the application.
 * Returns a UIView whose lv_obj is the active screen,
 * sized to 320×240 by convention.
 */
UIView *UIViewRoot_create(void);

#endif
