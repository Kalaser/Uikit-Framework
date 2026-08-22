#ifndef UIKIT_THEME_H
#define UIKIT_THEME_H

#include "lvgl.h"
#include <stdint.h>

/**
 * @file UITheme.h
 * @brief Global theme — a shared style layer to avoid hard-coding colors
 *        and fonts throughout the application.
 *
 * Components obtain their default look from the active theme instead of
 * defining every visual parameter locally, enabling global skin changes
 * without touching application logic.
 */

typedef struct UITheme {
    /* Colors (0xRRGGBB) */
    uint32_t background_color;
    uint32_t foreground_color;
    uint32_t primary_color;
    uint32_t secondary_color;

    /* Geometry */
    uint16_t corner_radius;

    /* Fonts (may be NULL → LVGL defaults) */
    const lv_font_t *font_small;
    const lv_font_t *font_normal;
    const lv_font_t *font_large;
} UITheme;

/**
 * Get the default theme.
 * @return Pointer to the global theme instance (never NULL).
 */
UITheme *UITheme_get_default(void);

/**
 * Replace the active theme with a copy of `theme`.
 * Pass NULL to restore the built-in defaults.
 */
void UITheme_set_default(const UITheme *theme);

/**
 * Reset the active theme to the built-in defaults.
 */
void UITheme_reset(void);

/**
 * Initialize the theme subsystem.
 * Called automatically by UIKit_init(); safe to call again.
 */
void UITheme_init(void);

#endif /* UIKIT_THEME_H */
