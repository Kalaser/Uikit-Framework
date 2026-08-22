#include "UITheme.h"

static UITheme s_default_theme = {
    .background_color = 0xF2F2F7,
    .foreground_color = 0x000000,
    .primary_color    = 0x007AFF,
    .secondary_color  = 0x8E8E93,
    .corner_radius    = 8,
    .font_small       = NULL,
    .font_normal      = NULL,
    .font_large       = NULL,
};

static UITheme s_theme = {
    .background_color = 0xF2F2F7,
    .foreground_color = 0x000000,
    .primary_color    = 0x007AFF,
    .secondary_color  = 0x8E8E93,
    .corner_radius    = 8,
    .font_small       = NULL,
    .font_normal      = NULL,
    .font_large       = NULL,
};

static bool s_initialized = false;

void UITheme_init(void)
{
    if (s_initialized) {
        return;
    }
    s_initialized = true;
    /* Load embedded defaults (font pointers resolved lazily) */
    s_theme = s_default_theme;
}

UITheme *UITheme_get_default(void)
{
    if (!s_initialized) {
        UITheme_init();
    }
    return &s_theme;
}

void UITheme_set_default(const UITheme *theme)
{
    if (!s_initialized) {
        UITheme_init();
    }
    if (theme) {
        s_theme = *theme;
    }
}

void UITheme_reset(void)
{
    if (!s_initialized) {
        UITheme_init();
    }
    s_theme = s_default_theme;
}
