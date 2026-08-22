/**
 * Example: Navigation — stack navigation with view controllers
 * (README §4 "navigation").
 *
 * Builds a UINavigationController with a root controller, pushes a
 * second screen, then pops back.
 */

#include "UIKit.h"
#include <stdio.h>
#include <assert.h>

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
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res  = 320;
    disp_drv.ver_res  = 240;
    disp_drv.flush_cb = flush_cb;
    lv_disp_drv_register(&disp_drv);
}

/* ── HomeViewController (subclass by embedding UIViewController) ──────────── */

typedef struct {
    UIViewController base;
} HomeViewController;

static UIView *home_load_view(UIViewController *self)
{
    HomeViewController *home = (HomeViewController *)self;
    UIView *view = UIView_create(NULL);
    UIView_set_background_color(view, 0xFFFFFF);

    UILabel *label = UILabel_create(view);
    UILabel_set_text(label, "Home Screen");
    UILabel_set_alignment(label, 1);
    UIView_set_frame(&label->base, 0, 20, 320, 30);

    /* Reuse the label pointer stored on the controller */
    (void)home;
    return view;
}

static void home_on_resume(UIViewController *self)
{
    (void)self;
    printf("Home resumed\n");
}

static HomeViewController *home_create(void)
{
    HomeViewController *home = UIKit_calloc(1, sizeof(HomeViewController));
    UIViewController_init(&home->base, NULL);
    home->base.load_view = home_load_view;
    home->base.onResume  = home_on_resume;
    return home;
}

/* ── SettingsViewController ───────────────────────────────────────────────── */

typedef struct {
    UIViewController base;
} SettingsViewController;

static UIView *settings_load_view(UIViewController *self)
{
    (void)self;
    UIView *view = UIView_create(NULL);
    UIView_set_background_color(view, 0xF2F2F7);

    UILabel *label = UILabel_create(view);
    UILabel_set_text(label, "Settings Screen");
    UILabel_set_alignment(label, 1);
    UIView_set_frame(&label->base, 0, 20, 320, 30);

    return view;
}

static SettingsViewController *settings_create(void)
{
    SettingsViewController *s = UIKit_calloc(1, sizeof(SettingsViewController));
    UIViewController_init(&s->base, NULL);
    s->base.load_view = settings_load_view;
    return s;
}

/* ── Main ─────────────────────────────────────────────────────────────────── */

int main(void)
{
    lv_init();
    setup_display();
    UIKit_init();

    HomeViewController *home = home_create();
    SettingsViewController *settings = settings_create();
    UIViewController_set_title(&home->base, "Home");
    UIViewController_set_title(&settings->base, "Settings");

    UINavigationController *nav = UINavigationController_create(&home->base);
    UINavigationController_push(nav, &settings->base);
    assert(UINavigationController_count(nav) == 2);
    assert(UINavigationController_top(nav) == &settings->base);

    for (int i = 0; i < 30; i++) {
        lv_tick_inc(5);
        lv_timer_handler();
        UIKit_process();
    }

    UIViewController *popped = UINavigationController_pop(nav);
    assert(popped == &settings->base);
    assert(UINavigationController_count(nav) == 1);

    UINavigationController_destroy(nav);   /* destroys home */
    UIViewController_destroy(&settings->base);

    printf("Navigation example finished.\n");
    return 0;
}
