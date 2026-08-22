#ifndef WIN_DRIVER_H
#define WIN_DRIVER_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file win_driver.h
 * @brief Windows GDI display + input backend for LVGL.
 *
 * Creates a 480x800 window, renders LVGL through a 16bpp RGB565 DIB
 * double buffer, and feeds mouse + keyboard input back into LVGL.
 */

extern int g_win_width;
extern int g_win_height;

/**
 * Create the window and register the LVGL display / pointer / keypad
 * drivers. Call after lv_init().
 *
 * @param smoke_frames  >0 → auto-close after N timer frames (CI/smoke mode);
 *                      0 → run until the window is closed.
 * @return 0 on success, -1 on failure.
 */
int WinDriver_init(int smoke_frames);

/**
 * Run the Windows message loop (blocks until the window closes,
 * or until the smoke frame budget is exhausted).
 */
void WinDriver_run(void);

/**
 * Register a callback fired whenever the client area is resized
 * (after LVGL's resolution has been updated). May be NULL.
 */
void WinDriver_set_resize_callback(void (*cb)(int w, int h));

/**
 * Destroy the window and release GDI resources.
 */
void WinDriver_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* WIN_DRIVER_H */
