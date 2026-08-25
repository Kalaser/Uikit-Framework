/**
 * @file launcher_theme.h
 * @brief Design tokens & shared style helpers for the launcher and its apps.
 *
 * Material-You-style dark theme: layered surfaces, muted text hierarchy,
 * consistent radii/spacing. Every app should pull colors/sizes from here
 * instead of hard-coding hex values, so the whole launcher can be re-themed
 * by editing a handful of constants.
 */

#ifndef LAUNCHER_THEME_H
#define LAUNCHER_THEME_H

#include "UIKit.hpp"
#include <cstdint>

namespace launcher_theme {

/* ── Color tokens (Material dark palette) ─────────────────────────────────── */

constexpr uint32_t kBg          = 0x0B1220;  /* page background            */
constexpr uint32_t kWallpaper   = 0x0F172A;  /* launcher wallpaper         */
constexpr uint32_t kSurface     = 0x1E293B;  /* cards / nav / dock         */
constexpr uint32_t kSurfaceAlt  = 0x273549;  /* raised surfaces / inputs   */
constexpr uint32_t kBorder      = 0x334155;  /* hairline separators        */
constexpr uint32_t kTextHi      = 0xF8FAFC;  /* primary text               */
constexpr uint32_t kTextMid     = 0x94A3B8;  /* secondary text             */
constexpr uint32_t kTextLow     = 0x64748B;  /* hints / captions           */
constexpr uint32_t kAccent      = 0x22D3EE;  /* brand accent (cyan)        */
constexpr uint32_t kAccentAlt   = 0x34D399;  /* secondary accent (green)   */
constexpr uint32_t kSuccess     = 0x4ADE80;  /* battery / positive         */

/* ── Radius tokens ────────────────────────────────────────────────────────── */

constexpr int32_t kRadiusIcon  = 24;   /* app icon containers        */
constexpr int32_t kRadiusCard  = 16;   /* grouped cards              */
constexpr int32_t kRadiusCtl   = 12;   /* small controls             */
constexpr int32_t kRadiusPill  = 22;   /* search bar / buttons       */

/* ── Spacing tokens ───────────────────────────────────────────────────────── */

constexpr int32_t kSpaceXl = 24;
constexpr int32_t kSpaceLg = 20;
constexpr int32_t kSpaceMd = 16;
constexpr int32_t kSpaceSm = 12;
constexpr int32_t kSpaceXs = 8;

/* ── Font tokens (LVGL built-in Montserrat) ──────────────────────────────── */

static const lv_font_t *const kFontH1   = &lv_font_montserrat_28;  /* page titles      */
static const lv_font_t *const kFontH2   = &lv_font_montserrat_20;  /* icon glyphs      */
static const lv_font_t *const kFontBody = &lv_font_montserrat_16;  /* row labels       */
static const lv_font_t *const kFontCap  = &lv_font_montserrat_14;  /* captions / dock  */

/* ── Style helpers ────────────────────────────────────────────────────────── */

/** Apply the standard card look: surface fill, rounded corners, soft shadow. */
inline void style_card(uikit::UIView &v, int32_t radius = kRadiusCard,
                       uint32_t bg = kSurface)
{
    v.setBackgroundColor(bg);
    v.setCornerRadius(radius);
    lv_obj_t *obj = ::UIView_native(v.native());
    lv_obj_set_style_shadow_width(obj, 16, 0);
    lv_obj_set_style_shadow_color(obj, lv_color_hex(0x000000), 0);
    lv_obj_set_style_shadow_opa(obj, 60, 0);
}

/** Give a button a pressed-state background so taps feel tactile.
 *  NOTE: no shadows here — several buttons use a half-pill radius
 *  (radius == height/2), and LVGL 8.3 hangs in the draw layer when a
 *  half-circle corner is combined with a shadow during partial redraws. */
inline void style_pressed(uikit::UIButton &btn, uint32_t pressed_color)
{
    lv_obj_t *obj = ::UIView_native(btn.native());
    lv_obj_set_style_bg_color(obj, lv_color_hex(pressed_color),
                              LV_PART_MAIN | LV_STATE_PRESSED);
}

/** Secondary text label (icon captions, hints). */
inline void style_caption(uikit::UILabel &lbl, uint32_t color = kTextMid)
{
    lbl.setColor(color);
    lbl.setFont(kFontCap);
}

} /* namespace launcher_theme */

#endif /* LAUNCHER_THEME_H */
