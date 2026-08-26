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
 * host machine. It also uses UIViewController + UINavigationController so
 * the simple demo follows the same Activity/page lifecycle as real apps.
 * On real hardware, replace setup_display() with your platform's
 * display/input driver initialization.
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

/* ── Demo activity/page ───────────────────────────────────────────────────── */

typedef struct DemoActivity {
    UIViewController base;
    UILabel *title;
    UIButton *button;
    UILabel *status;
    UISwitch *sw;
    UISlider *slider;
    UIProgressView *progress;
    UIPageControl *pages;
    UITextField *field;
} DemoActivity;

static UIView *demo_load_view(UIViewController *controller)
{
    (void)controller;
    return UIViewRoot_create();
}

static void demo_on_create(UIViewController *controller)
{
    DemoActivity *activity = (DemoActivity *)controller;
    UIView *root = UIViewController_get_view(controller);
    if (!root) {
        return;
    }

    UIView_set_background_color(root, 0xF2F2F7);

    activity->title = UILabel_create(root);
    UILabel_set_text(activity->title, "UIKit Demo");
    UILabel_set_color(activity->title, 0x000000);
    UILabel_set_alignment(activity->title, 1);
    UIView_set_frame(&activity->title->base, 0, 10, 320, 30);

    activity->button = UIButton_create(root);
    UIButton_set_title(activity->button, "Tap me");
    UIButton_set_title_color(activity->button, 0xFFFFFF);
    UIButton_set_background_color(activity->button, 0x007AFF);
    UIView_set_corner_radius(&activity->button->base, 8);
    UIView_set_frame(&activity->button->base, 40, 50, 240, 40);

    activity->status = UILabel_create(root);
    UILabel_set_text(activity->status, "Ready.");
    UILabel_set_color(activity->status, 0x555555);
    UIView_set_frame(&activity->status->base, 40, 95, 240, 20);
    UIButton_on_click(activity->button, on_button_click, activity->status);

    activity->sw = UISwitch_create(root);
    UIView_set_position(&activity->sw->base, 40, 125);
    UISwitch_on_change(activity->sw, on_switch_change, activity->status);

    activity->slider = UISlider_create(root);
    UISlider_set_range(activity->slider, 0, 100);
    UISlider_set_value(activity->slider, 50);
    UIView_set_frame(&activity->slider->base, 40, 165, 240, 10);

    activity->progress = UIProgressView_create(root);
    UIProgressView_set_value(activity->progress, 50);
    UIProgressView_set_progress_color(activity->progress, 0x34C759);
    UIView_set_frame(&activity->progress->base, 40, 185, 240, 8);
    UISlider_on_change(activity->slider, on_slider_change, activity->progress);

    activity->pages = UIPageControl_create(root);
    UIPageControl_set_page_count(activity->pages, 3);
    UIPageControl_set_current_page(activity->pages, 1);
    UIView_set_position(&activity->pages->base, 110, 210);

    activity->field = UITextField_create(root);
    UITextField_set_placeholder(activity->field, "Type here...");
    UIView_set_frame(&activity->field->base, 40, 130, 120, 30);
}

static void demo_on_resume(UIViewController *controller)
{
    (void)controller;
    printf("[DemoActivity] onResume\n");
}

static void demo_on_destroy(UIViewController *controller)
{
    DemoActivity *activity = (DemoActivity *)controller;
    UITextField_destroy(activity->field);
    UIPageControl_destroy(activity->pages);
    UIProgressView_destroy(activity->progress);
    UISlider_destroy(activity->slider);
    UISwitch_destroy(activity->sw);
    UILabel_destroy(activity->status);
    UIButton_destroy(activity->button);
    UILabel_destroy(activity->title);
}

static DemoActivity *demo_activity_create(void)
{
    DemoActivity *activity = UIKit_calloc(1, sizeof(DemoActivity));
    if (!activity) {
        return NULL;
    }
    UIViewController_init(&activity->base, NULL);
    activity->base.load_view = demo_load_view;
    activity->base.onCreate = demo_on_create;
    activity->base.onResume = demo_on_resume;
    activity->base.onDestroy = demo_on_destroy;
    return activity;
}

/* ── Main ─────────────────────────────────────────────────────────────────── */

int main(void)
{
    lv_init();
    setup_display();

    UIKit_init();

    DemoActivity *activity = demo_activity_create();
    if (!activity) {
        printf("DemoActivity allocation failed.\n");
        return 1;
    }
    UINavigationController *nav = UINavigationController_create(&activity->base);
    if (!nav) {
        UIViewController_destroy(&activity->base);
        printf("UINavigationController_create failed.\n");
        return 1;
    }

    /* Main loop (host build: drive LVGL's tick manually) */
    for (int i = 0; i < 100; i++) {
        lv_tick_inc(5);
        lv_timer_handler();
        UIKit_process();
    }

    UINavigationController_destroy(nav);

    printf("UIKitDemo finished successfully.\n");
    return 0;
}
