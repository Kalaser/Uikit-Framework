#ifndef UIKIT_PLATFORM_H
#define UIKIT_PLATFORM_H

#include <stdint.h>
#include <stddef.h>

/**
 * @file UIKitPlatform.h
 * @brief Platform abstraction layer.
 *
 * The core framework depends only on this small platform interface,
 * allowing integration with bare metal, FreeRTOS, NuttX, Zephyr,
 * Linux, ESP-IDF, STM32 HAL, etc., without changing the UI component API.
 *
 * Override these by defining UIKIT_PLATFORM_CUSTOM and providing your
 * own implementations before linking.
 */

/* ── Time ─────────────────────────────────────────────────────────────────── */

#ifndef UIKIT_PLATFORM_CUSTOM
/**
 * Get the current tick count in milliseconds.
 * Default: uses LVGL's lv_tick_get().
 */
uint32_t UIKit_get_tick_ms(void);
#endif

/* ── Memory ───────────────────────────────────────────────────────────────── */

#ifndef UIKIT_PLATFORM_CUSTOM
/**
 * Allocate memory.
 * Default: wraps standard malloc.
 */
void *UIKit_malloc(size_t size);

/**
 * Free memory.
 * Default: wraps standard free.
 */
void UIKit_free(void *ptr);

/**
 * Zero-initialized allocation.
 * Default: wraps calloc.
 */
void *UIKit_calloc(size_t count, size_t size);
#endif

/* ── Synchronization ──────────────────────────────────────────────────────── */

#ifndef UIKIT_PLATFORM_CUSTOM
/**
 * Lock the UI mutex (for thread safety).
 * Default: no-op (single-threaded).
 */
void UIKit_lock(void);

/**
 * Unlock the UI mutex.
 * Default: no-op (single-threaded).
 */
void UIKit_unlock(void);
#endif

/* ── UI Dispatch ──────────────────────────────────────────────────────────── */

/**
 * Dispatch a function to run on the UI thread.
 * On bare metal this calls the function directly.
 * On RTOS platforms this posts to the UI task's message queue.
 *
 * @param func    Function to execute.
 * @param user_data  Context passed to the function.
 */
#ifndef UIKIT_PLATFORM_CUSTOM
void UIKit_dispatch(void (*func)(void *user_data), void *user_data);
#endif

#endif /* UIKIT_PLATFORM_H */
