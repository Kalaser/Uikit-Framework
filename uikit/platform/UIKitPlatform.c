#include "UIKitPlatform.h"
#include "lvgl.h"
#include <stdlib.h>

/* ── Time ─────────────────────────────────────────────────────────────────── */

uint32_t UIKit_get_tick_ms(void)
{
    return lv_tick_get();
}

/* ── Memory ───────────────────────────────────────────────────────────────── */

void *UIKit_malloc(size_t size)
{
    return malloc(size);
}

void UIKit_free(void *ptr)
{
    free(ptr);
}

void *UIKit_calloc(size_t count, size_t size)
{
    return calloc(count, size);
}

/* ── Synchronization ──────────────────────────────────────────────────────── */

void UIKit_lock(void)
{
    /* No-op on single-threaded bare-metal targets.
     * On FreeRTOS this would be xSemaphoreTake(mutex, portMAX_DELAY). */
}

void UIKit_unlock(void)
{
    /* No-op on single-threaded bare-metal targets. */
}

/* ── UI Dispatch ──────────────────────────────────────────────────────────── */

void UIKit_dispatch(void (*func)(void *user_data), void *user_data)
{
    /* On bare metal, call directly.
     * On RTOS, post to the UI task's queue. */
    if (func) {
        func(user_data);
    }
}
