#include "UIKit.h"

static bool uikit_initialized = false;

void UIKit_init(void)
{
    if (uikit_initialized) {
        return;
    }
    uikit_initialized = true;

    /* Initialize subsystems */
    UITheme_init();
}

void UIKit_process(void)
{
    /* Currently no framework-level periodic work.
     * LVGL's lv_timer_handler() is called by the application. */
}
