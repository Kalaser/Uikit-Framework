/**
 * UIKitDemo — demonstrates the UIKit-Framework component set.
 *
 * Architecture (key design principle):
 *
 *   UIKit  = Application UI Abstraction
 *   LVGL   = Rendering / UI Engine
 *   Display/Input/Timer = Hardware / OS layer
 *
 * UIView is NOT a widget library — it is a thin wrapper around lv_obj_t.
 * Every UIKit component maps one-to-one onto an LVGL primitive.
 *
 * This demo registers a headless (no-op flush) display so it runs on any
 * host machine. On real hardware, replace setup_display() with your
 * platform's display/input driver initialization.
 */

#include "UIKit.h"
#include "UIKitPlatform.h"
#include <stdio.h>

/* ── Headless display driver (host builds only) ─────────────────────────── */

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[320 * 20];
static lv_disp_drv_t disp_drv;

static void flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    (void)drv; (void)area; (void)color_p;
    lv_disp_flush_ready(drv);
}

static void setup_display(void)
{
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, 320 * 20);
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &draw_buf;
    disp_drv.hor_res    = 320;
    disp_drv.ver_res    = 240;
    disp_drv.flush_cb   = flush_cb;
    disp_drv.antialiasing = 1;
    lv_disp_drv_register(&disp_drv);
}

/* ── Event handlers ───────────────────────────────────────────────────────── */

static void on_button_click(UIEvent *event, void *user_data)
{
    UILabel *status = (UILabel *)user_data;
    if (status && event->type == UIEVENT_CLICK) {
        UILabel_set_text(status, "Button clicked!");
    }
}

static void on_switch_change(UIEvent *event, void *user_data)
{
    UILabel *status = (UILabel *)user_data;
    if (status) {
        UILabel_set_text(status, event->value ? "Switch: ON" : "Switch: OFF");
    }
}

static void on_slider_change(UIEvent *event, void *user_data)
{
    UIProgressView *progress = (UIProgressView *)user_data;
    if (progress) {
        UIProgressView_set_value(progress, event->value);
    }
}

/* ── Build the demo UI ────────────────────────────────────────────────────── */

static UILabel *g_status;

static void app_ui_init(void)
{
    UIView *root = UIViewRoot_create();
    if (!root) {
        return;
    }
    UIView_set_background_color(root, 0xF2F2F7);

    /* Title */
    UILabel *title = UILabel_create(root);
    UILabel_set_text(title, "UIKit Demo");
    UILabel_set_color(title, 0x000000);
    UILabel_set_alignment(title, 1);
    UIView_set_frame(&title->base, 0, 10, 320, 30);

    /* Button */
    UIButton *button = UIButton_create(root);
    UIButton_set_title(button, "Tap me");
    UIButton_set_title_color(button, 0xFFFFFF);
    UIButton_set_background_color(button, 0x007AFF);
    UIView_set_corner_radius(&button->base, 8);
    UIView_set_frame(&button->base, 40, 50, 240, 40);
    g_status = UILabel_create(root);
    UILabel_set_text(g_status, "Ready.");
    UILabel_set_color(g_status, 0x555555);
    UIView_set_frame(&g_status->base, 40, 95, 240, 20);
    UIButton_on_click(button, on_button_click, g_status);

    /* Switch */
    UISwitch *sw = UISwitch_create(root);
    UIView_set_position(&sw->base, 40, 125);
    UISwitch_on_change(sw, on_switch_change, g_status);

    /* Slider + progress */
    UISlider *slider = UISlider_create(root);
    UISlider_set_range(slider, 0, 100);
    UISlider_set_value(slider, 50);
    UIView_set_frame(&slider->base, 40, 165, 240, 10);

    UIProgressView *progress = UIProgressView_create(root);
    UIProgressView_set_value(progress, 50);
    UIProgressView_set_progress_color(progress, 0x34C759);
    UIView_set_frame(&progress->base, 40, 185, 240, 8);
    UISlider_on_change(slider, on_slider_change, progress);

    /* Page control */
    UIPageControl *pages = UIPageControl_create(root);
    UIPageControl_set_page_count(pages, 3);
    UIPageControl_set_current_page(pages, 1);
    UIView_set_position(&pages->base, 110, 210);

    /* Text field */
    UITextField *field = UITextField_create(root);
    UITextField_set_placeholder(field, "Type here...");
    UIView_set_frame(&field->base, 40, 130, 120, 30);
}

/* ── Main ─────────────────────────────────────────────────────────────────── */

int main(void)
{
    lv_init();
    setup_display();

    UIKit_init();
    app_ui_init();

    /* Main loop (host build: drive LVGL's tick manually) */
    for (int i = 0; i < 100; i++) {
        lv_tick_inc(5);
        lv_timer_handler();
        UIKit_process();
    }

    printf("UIKitDemo finished successfully.\n");
    return 0;
}
