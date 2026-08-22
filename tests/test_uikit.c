/**
 * UIKitTests — smoke tests for the UIKit-Framework core, components
 * and controller layers.
 *
 * Uses a headless LVGL display so it runs on any host.
 */

#include "UIKit.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* ── Headless display ─────────────────────────────────────────────────────── */

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[320 * 20];
static lv_disp_drv_t disp_drv;

static void flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    (void)drv; (void)area; (void)color_p;
    lv_disp_flush_ready(drv);
}

static int tests_run = 0;

#define RUN_TEST(fn)                                    \
    do {                                                \
        tests_run++;                                    \
        printf("  [RUN ] %s\n", #fn);                   \
        fn();                                           \
        printf("  [ OK ] %s\n", #fn);                   \
    } while (0)

/* ── UIKitResult ───────────────────────────────────────────────────────────── */

static void test_uikitresult_to_string(void)
{
    assert(strcmp(UIKitResult_to_string(UIKIT_OK), "OK") == 0);
    assert(strcmp(UIKitResult_to_string(UIKIT_ERROR_INVALID_ARG),
                  "Invalid argument") == 0);
    assert(UIKitResult_to_string((UIKitResult)999) != NULL);
}

/* ── UIView ────────────────────────────────────────────────────────────────── */

static void test_uiview_create_destroy(void)
{
    UIView *view = UIView_create(NULL);
    assert(view != NULL);
    assert(view->lv_obj != NULL);
    assert(UIView_get_superview(view) == NULL);

    UIView *child = UIView_create(view);
    assert(child != NULL);
    assert(UIView_get_superview(child) == view);

    UIView_destroy(child);
    UIView_destroy(view);
}

static void test_uiview_frame(void)
{
    UIView *view = UIView_create(NULL);
    assert(view != NULL);

    UIView_set_frame(view, 10, 20, 100, 50);
    int32_t x = -1, y = -1, w = -1, h = -1;
    UIView_get_frame(view, &x, &y, &w, &h);
    assert(x == 10 && y == 20);
    assert(w == 100 && h == 50);

    /* NULL output pointers must be tolerated */
    UIView_get_frame(view, NULL, NULL, NULL, NULL);

    UIView_destroy(view);
}

static void test_uiview_appearance(void)
{
    UIView *view = UIView_create(NULL);
    assert(view != NULL);

    UIView_set_background_color(view, 0xFF0000);
    UIView_set_border(view, 0x00FF00, 2);
    UIView_set_corner_radius(view, 5);
    UIView_set_opacity(view, 128);

    UIView_destroy(view);
}

static void test_uiview_visibility_hierarchy(void)
{
    UIView *parent = UIView_create(NULL);
    UIView *child  = UIView_create(NULL);
    assert(parent && child);

    UIView_set_hidden(child, true);
    assert(UIView_is_hidden(child));
    UIView_set_hidden(child, false);
    assert(!UIView_is_hidden(child));

    UIView_add_subview(parent, child);
    assert(UIView_get_superview(child) == parent);

    UIView_remove_from_superview(child);
    assert(UIView_get_superview(child) == NULL);

    UIView_destroy(child);
    UIView_destroy(parent);
}

static void test_uiview_tag(void)
{
    UIView *view = UIView_create(NULL);
    int marker = 42;

    assert(UIView_get_tag(view) == NULL);
    UIView_set_tag(view, &marker);
    assert(UIView_get_tag(view) == &marker);
    assert(*(int *)UIView_get_tag(view) == 42);

    UIView_destroy(view);
}

/* ── Event system ──────────────────────────────────────────────────────────── */

static int g_click_count = 0;

static void click_handler(UIEvent *event, void *user_data)
{
    (void)event;
    g_click_count += (int)(intptr_t)user_data;
}

static void test_uiview_callbacks(void)
{
    UIView *view = UIView_create(NULL);
    assert(view != NULL);

    UIView_add_callback(view, UIEVENT_CLICK, click_handler, (void *)1);
    UIView_add_callback(view, UIEVENT_CLICK, click_handler, (void *)10);

    /* Simulate the LVGL event */
    g_click_count = 0;
    lv_event_send(view->lv_obj, LV_EVENT_SHORT_CLICKED, NULL);
    assert(g_click_count == 11);

    /* Remove one callback (the first match in prepend order = user_data 10) */
    UIView_remove_callback(view, UIEVENT_CLICK, click_handler);
    g_click_count = 0;
    lv_event_send(view->lv_obj, LV_EVENT_SHORT_CLICKED, NULL);
    assert(g_click_count == 1);

    /* NULL safety */
    UIView_add_callback(NULL, UIEVENT_CLICK, click_handler, NULL);
    UIView_remove_callback(NULL, UIEVENT_CLICK, click_handler);

    UIView_destroy(view);
}

/* ── UILabel ───────────────────────────────────────────────────────────────── */

static void test_uilabel(void)
{
    UILabel *label = UILabel_create(NULL);
    assert(label != NULL);

    UILabel_set_text(label, "Hello");
    assert(strcmp(UILabel_get_text(label), "Hello") == 0);

    UILabel_set_color(label, 0xFF0000);
    UILabel_set_alignment(label, 1);
    UILabel_set_font(label, &lv_font_montserrat_14);

    /* NULL safety */
    UILabel_set_text(NULL, "x");
    UILabel_set_text(label, NULL);
    assert(UILabel_get_text(NULL) == NULL);

    UILabel_destroy(label);
}

/* ── UIButton ──────────────────────────────────────────────────────────────── */

static void test_uibutton(void)
{
    UIButton *button = UIButton_create(NULL);
    assert(button != NULL);

    UIButton_set_title(button, "OK");
    assert(strcmp(UIButton_get_title(button), "OK") == 0);

    UIButton_set_title_color(button, 0xFFFFFF);
    UIButton_set_background_color(button, 0x007AFF);

    assert(UIButton_get_title(NULL) == NULL);

    UIButton_destroy(button);
}

/* ── UISwitch ──────────────────────────────────────────────────────────────── */

static void test_uiswitch(void)
{
    UISwitch *sw = UISwitch_create(NULL);
    assert(sw != NULL);

    assert(!UISwitch_is_on(sw));
    UISwitch_set_on(sw, true);
    assert(UISwitch_is_on(sw));
    UISwitch_set_on_instant(sw, false);
    assert(!UISwitch_is_on(sw));

    UISwitch_set_knob_color(sw, 0xFFFFFF);
    UISwitch_set_on_color(sw, 0x34C759);

    assert(!UISwitch_is_on(NULL));

    UISwitch_destroy(sw);
}

/* ── UISlider ──────────────────────────────────────────────────────────────── */

static void test_uislider(void)
{
    UISlider *slider = UISlider_create(NULL);
    assert(slider != NULL);

    UISlider_set_range(slider, 0, 100);
    UISlider_set_value(slider, 42);
    assert(UISlider_get_value(slider) == 42);

    UISlider_set_vertical(slider, true);
    UISlider_set_indicator_color(slider, 0x007AFF);
    UISlider_set_knob_color(slider, 0xFFFFFF);

    assert(UISlider_get_value(NULL) == 0);

    UISlider_destroy(slider);
}

/* ── UITextField ───────────────────────────────────────────────────────────── */

static void test_uitextfield(void)
{
    UITextField *field = UITextField_create(NULL);
    assert(field != NULL);

    UITextField_set_text(field, "abc");
    assert(strcmp(UITextField_get_text(field), "abc") == 0);

    UITextField_set_placeholder(field, "hint");
    UITextField_set_max_length(field, 8);
    UITextField_set_password_mode(field, true);
    UITextField_set_password_mode(field, false);
    UITextField_set_editable(field, false);
    UITextField_set_editable(field, true);

    assert(UITextField_get_text(NULL) == NULL);

    UITextField_destroy(field);
}

/* ── UIImageView ───────────────────────────────────────────────────────────── */

static void test_uiimageview(void)
{
    UIImageView *view = UIImageView_create(NULL);
    assert(view != NULL);

    UIImageView_set_image(view, LV_SYMBOL_OK);
    UIImageView_set_scale(view, 128);
    UIImageView_set_rotation(view, 450);
    UIImageView_set_antialias(view, true);

    UIImageView_destroy(view);
}

/* ── UIScrollView ──────────────────────────────────────────────────────────── */

static void test_uiscrollview(void)
{
    UIScrollView *scroll = UIScrollView_create(NULL);
    assert(scroll != NULL);

    UIView *child = UIView_create(&scroll->base);
    assert(child != NULL);
    UIView_set_frame(child, 0, 0, 100, 600);

    UIScrollView_set_vertical_scroll(scroll, false);
    UIScrollView_set_vertical_scroll(scroll, true);
    UIScrollView_set_horizontal_scroll(scroll, true);
    UIScrollView_set_scrollbar_visible(scroll, false);
    UIScrollView_set_scrollbar_visible(scroll, true);
    UIScrollView_scroll_to_view(scroll, child);
    UIScrollView_scroll_to(scroll, 0, 100);

    UIView_destroy(child);
    UIScrollView_destroy(scroll);
}

/* ── UIProgressView ────────────────────────────────────────────────────────── */

static void test_uiprogressview(void)
{
    UIProgressView *view = UIProgressView_create(NULL);
    assert(view != NULL);

    UIProgressView_set_range(view, 0, 100);
    UIProgressView_set_value_instant(view, 60);
    assert(UIProgressView_get_value(view) == 60);
    UIProgressView_set_value(view, 30);
    assert(UIProgressView_get_value(view) == 30);

    UIProgressView_set_track_color(view, 0xE5E5EA);
    UIProgressView_set_progress_color(view, 0x007AFF);
    UIProgressView_set_vertical(view, true);

    assert(UIProgressView_get_value(NULL) == 0);

    UIProgressView_destroy(view);
}

/* ── UIPageControl ─────────────────────────────────────────────────────────── */

static void test_uipagecontrol(void)
{
    UIPageControl *control = UIPageControl_create(NULL);
    assert(control != NULL);

    UIPageControl_set_page_count(control, 3);
    assert(UIPageControl_get_page_count(control) == 3);
    assert(UIPageControl_get_current_page(control) == 0);

    UIPageControl_set_current_page(control, 2);
    assert(UIPageControl_get_current_page(control) == 2);

    /* Out-of-range must be ignored */
    UIPageControl_set_current_page(control, 99);
    assert(UIPageControl_get_current_page(control) == 2);

    UIPageControl_set_current_color(control, 0xFF0000);
    UIPageControl_set_page_color(control, 0xCCCCCC);

    assert(UIPageControl_get_page_count(NULL) == 0);

    UIPageControl_destroy(control);
}

/* ── UIViewController ──────────────────────────────────────────────────────── */

typedef struct {
    UIViewController base;
    int create_count;
    int start_count;
    int resume_count;
    int pause_count;
    int stop_count;
    int destroy_count;
} TestController;

static UIView *test_load_view(UIViewController *self)
{
    TestController *tc = (TestController *)self;
    UIView *view = UIView_create(NULL);
    UILabel *l = UILabel_create(view);
    UILabel_set_text(l, "TestController view");
    (void)tc;
    return view;
}

static void test_on_create(UIViewController *self)
{
    ((TestController *)self)->create_count++;
}

static void test_on_start(UIViewController *self)
{
    ((TestController *)self)->start_count++;
}

static void test_on_resume(UIViewController *self)
{
    ((TestController *)self)->resume_count++;
}

static void test_on_pause(UIViewController *self)
{
    ((TestController *)self)->pause_count++;
}

static void test_on_stop(UIViewController *self)
{
    ((TestController *)self)->stop_count++;
}

static void test_on_destroy(UIViewController *self)
{
    ((TestController *)self)->destroy_count++;
}

static TestController *test_controller_create(void)
{
    TestController *tc = UIKit_calloc(1, sizeof(TestController));
    UIViewController_init(&tc->base, NULL);
    tc->base.load_view = test_load_view;
    tc->base.onCreate  = test_on_create;
    tc->base.onStart   = test_on_start;
    tc->base.onResume  = test_on_resume;
    tc->base.onPause   = test_on_pause;
    tc->base.onStop    = test_on_stop;
    tc->base.onDestroy = test_on_destroy;
    return tc;
}

static void test_uiviewcontroller_lifecycle(void)
{
    TestController *tc = test_controller_create();

    UIView *view = UIViewController_get_view(&tc->base);
    assert(view != NULL);

    UIViewController_onCreate(&tc->base);
    assert(tc->create_count == 1);
    assert(tc->base.state == UIViewControllerState_Created);

    UIViewController_onStart(&tc->base);
    UIViewController_onResume(&tc->base);
    UIViewController_onPause(&tc->base);
    UIViewController_onStop(&tc->base);

    assert(tc->start_count == 1);
    assert(tc->resume_count == 1);
    assert(tc->pause_count == 1);
    assert(tc->stop_count == 1);
    assert(tc->base.state == UIViewControllerState_Stopped);

    UIViewController_destroy(&tc->base);
    assert(tc->destroy_count == 1);
}

static void test_uiviewcontroller_state_machine(void)
{
    /* onCreate is idempotent (repeat pushes never re-create) */
    TestController *a = test_controller_create();
    UIViewController_onCreate(&a->base);
    UIViewController_onCreate(&a->base);
    UIViewController_onCreate(&a->base);
    assert(a->create_count == 1);
    assert(a->base.state == UIViewControllerState_Created);
    UIViewController_destroy(&a->base);

    /* Illegal order: onResume before onStart is ignored */
    TestController *b = test_controller_create();
    UIViewController_onResume(&b->base);
    assert(b->resume_count == 0);
    assert(b->base.state == UIViewControllerState_None);
    UIViewController_destroy(&b->base);

    /* onPause without being resumed is ignored */
    TestController *c = test_controller_create();
    UIViewController_onCreate(&c->base);
    UIViewController_onPause(&c->base);
    assert(c->pause_count == 0);
    assert(c->base.state == UIViewControllerState_Created);
    UIViewController_destroy(&c->base);

    /* Re-entry path: Stopped → Started → Resumed */
    TestController *d = test_controller_create();
    UIViewController_onCreate(&d->base);
    UIViewController_onStart(&d->base);
    UIViewController_onResume(&d->base);
    UIViewController_onPause(&d->base);
    UIViewController_onStop(&d->base);
    UIViewController_onStart(&d->base);    /* re-enter */
    UIViewController_onResume(&d->base);
    assert(d->start_count == 2);
    assert(d->resume_count == 2);
    assert(d->pause_count == 1);
    assert(d->stop_count == 1);
    UIViewController_destroy(&d->base);
    assert(d->destroy_count == 1);

    /* onDestroy is idempotent */
    TestController *e = test_controller_create();
    UIViewController_onCreate(&e->base);
    UIViewController_onDestroy(&e->base);
    UIViewController_onDestroy(&e->base);
    assert(e->destroy_count == 1);
    assert(e->base.state == UIViewControllerState_Destroyed);
    UIViewController_destroy(&e->base);
    assert(e->destroy_count == 1);   /* no double callback */

    /* deinit does NOT free the struct (C++ member embedding) */
    TestController *f = test_controller_create();
    UIViewController_onCreate(&f->base);
    UIViewController_deinit(&f->base);
    UIViewController_deinit(&f->base);   /* idempotent */
    assert(f->destroy_count == 1);
    assert(f->base.view == NULL);        /* root view released */
    assert(f->base.state == UIViewControllerState_Destroyed);
    UIKit_free(f);

    /* destroy before any creation: no callbacks, struct freed */
    TestController *g = test_controller_create();
    UIViewController_onDestroy(&g->base);   /* None → Destroyed, no callback */
    assert(g->destroy_count == 0);
    assert(g->base.state == UIViewControllerState_Destroyed);
    UIKit_free(g);   /* onDestroy does not free — only destroy does */
}

/* ── Deprecated hook forwarding ───────────────────────────────────────────── */

static int g_legacy_will_appear = 0;

static void legacy_will_appear(UIViewController *self)
{
    (void)self;
    g_legacy_will_appear++;
}

static void test_uiviewcontroller_compat_hooks(void)
{
    TestController *tc = test_controller_create();
    tc->base.view_will_appear = legacy_will_appear;

    UIViewController_onCreate(&tc->base);
    UIViewController_onStart(&tc->base);   /* forwards to view_will_appear */
    assert(g_legacy_will_appear == 1);
    assert(tc->start_count == 1);

    /* Deprecated trigger is an alias for the state machine trigger */
    UIViewController_view_will_appear(&tc->base);   /* Created? no — now Started */
    assert(g_legacy_will_appear == 1);   /* invalid transition, ignored */

    UIViewController_destroy(&tc->base);
}

/* ── UINavigationController ────────────────────────────────────────────────── */

static void test_uinavigationcontroller(void)
{
    TestController *root = test_controller_create();
    TestController *second = test_controller_create();
    TestController *third = test_controller_create();

    UINavigationController *nav = UINavigationController_create(&root->base);
    assert(nav != NULL);
    assert(UINavigationController_count(nav) == 1);
    assert(UINavigationController_top(nav) == &root->base);

    /* Root: create → start → resume on entry */
    assert(root->create_count == 1);
    assert(root->start_count == 1);
    assert(root->resume_count == 1);
    assert(root->base.state == UIViewControllerState_Resumed);

    /* Push: root pauses/stops, second goes through full lifecycle */
    UINavigationController_push(nav, &second->base);
    assert(UINavigationController_count(nav) == 2);
    assert(UINavigationController_top(nav) == &second->base);
    assert(second->create_count == 1);
    assert(second->start_count == 1);
    assert(second->resume_count == 1);
    assert(root->pause_count == 1);
    assert(root->stop_count == 1);

    /* Pop: second pauses/stops but is NOT destroyed */
    UIViewController *popped = UINavigationController_pop(nav);
    assert(popped == &second->base);
    assert(UINavigationController_count(nav) == 1);
    assert(second->pause_count == 1);
    assert(second->stop_count == 1);
    assert(second->destroy_count == 0);
    assert(root->start_count == 2);    /* re-entry */
    assert(root->resume_count == 2);

    /* Pop on root-only stack is a no-op */
    assert(UINavigationController_pop(nav) == NULL);

    /* Re-push the same controller: onCreate NOT repeated */
    UINavigationController_push(nav, &second->base);
    assert(second->create_count == 1);
    assert(second->start_count == 2);
    assert(second->resume_count == 2);
    assert(root->pause_count == 2);
    assert(root->stop_count == 2);

    /* Push / pop-to-root */
    UINavigationController_push(nav, &third->base);
    assert(UINavigationController_count(nav) == 3);
    assert(UINavigationController_at(nav, 1) == &second->base);

    int popped_count = UINavigationController_pop_to_root(nav);
    assert(popped_count == 2);
    assert(UINavigationController_count(nav) == 1);
    assert(UINavigationController_top(nav) == &root->base);
    assert(second->pause_count == 2);
    assert(second->stop_count == 2);     /* already Stopped — no duplicate pause/stop */
    assert(third->pause_count == 1);
    assert(third->stop_count == 1);
    assert(root->resume_count == 3);

    /* Out-of-bounds access */
    assert(UINavigationController_at(nav, 5) == NULL);
    assert(UINavigationController_at(nav, -1) == NULL);

    /* Nav destroy tears down the remaining stack (root) */
    UINavigationController_destroy(nav);
    assert(root->destroy_count == 1);

    /* Popped controllers were NOT destroyed by the nav — caller owns them */
    UIViewController_destroy(&second->base);
    UIViewController_destroy(&third->base);
    assert(second->destroy_count == 1);
    assert(third->destroy_count == 1);
}

/* ── Theme ─────────────────────────────────────────────────────────────────── */

static void test_uitheme(void)
{
    UITheme *theme = UITheme_get_default();
    assert(theme != NULL);
    assert(theme->primary_color == 0x007AFF);

    UITheme custom;
    memset(&custom, 0, sizeof(custom));
    custom.background_color = 0x111111;
    custom.foreground_color = 0xEEEEEE;
    custom.primary_color    = 0xFF0000;
    custom.corner_radius    = 12;

    UITheme_set_default(&custom);
    assert(UITheme_get_default()->primary_color == 0xFF0000);

    /* NULL restores nothing but is safe */
    UITheme_set_default(NULL);

    UITheme_reset();
    assert(UITheme_get_default()->primary_color == 0x007AFF);
}

/* ── Animation ─────────────────────────────────────────────────────────────── */

static void test_uiview_animation(void)
{
    UIView *view = UIView_create(NULL);
    assert(view != NULL);
    UIView_set_frame(view, 0, 0, 100, 50);

    UIView_animate(view, UIANIM_PROP_X, 200, 100);
    UIView_animate(view, UIANIM_PROP_Y, 150, 100);
    UIView_animate(view, UIANIM_PROP_WIDTH, 120, 100);
    UIView_animate(view, UIANIM_PROP_HEIGHT, 60, 100);
    UIView_animate(view, UIANIM_PROP_OPACITY, 128, 100);

    UIView_stop_animations(view);

    /* NULL / invalid inputs are safe */
    UIView_animate(NULL, UIANIM_PROP_X, 0, 100);
    UIView_animate(view, (UIAnimationProperty)99, 0, 100);
    UIView_animate(view, UIANIM_PROP_X, 0, 0);
    UIView_stop_animations(NULL);

    UIView_destroy(view);
}

/* ── UIStackView ───────────────────────────────────────────────────────────── */

static void test_uistackview(void)
{
    UIStackView *stack = UIStackView_create(NULL);
    assert(stack != NULL);

    UIStackView_set_axis(stack, UIStackAxis_VERTICAL);
    UIStackView_set_axis(stack, UIStackAxis_HORIZONTAL);
    UIStackView_set_spacing(stack, 8);
    UIStackView_set_padding(stack, 12);
    UIStackView_set_alignment(stack, 1);

    UIView *a = UIView_create(NULL);
    UIView *b = UIView_create(NULL);
    UIView_set_size(a, 40, 20);
    UIView_set_size(b, 40, 20);
    UIStackView_add_arranged_subview(stack, a);
    UIStackView_add_arranged_subview(stack, b);
    assert(UIView_get_superview(a) == &stack->base);

    UIView_destroy(a);
    UIView_destroy(b);
    UIStackView_destroy(stack);
}

/* ── API aliases (README naming) ───────────────────────────────────────────── */

static void test_api_aliases(void)
{
    UIButton *button = UIButton_create(NULL);
    assert(button != NULL);

    UIButton_set_text(button, "Start");
    assert(strcmp(UIButton_get_text(button), "Start") == 0);
    assert(strcmp(UIButton_get_title(button), "Start") == 0);

    g_click_count = 0;
    UIButton_set_action(button, click_handler, (void *)3);
    lv_event_send(button->base.lv_obj, LV_EVENT_SHORT_CLICKED, NULL);
    assert(g_click_count == 3);

    UIButton_destroy(button);

    /* UIView_remove_subview */
    UIView *parent = UIView_create(NULL);
    UIView *child  = UIView_create(NULL);
    UIView_add_subview(parent, child);
    assert(UIView_get_superview(child) == parent);
    UIView_remove_subview(parent, child);
    assert(UIView_get_superview(child) == NULL);
    UIView_destroy(child);
    UIView_destroy(parent);
}

/* ── Main ──────────────────────────────────────────────────────────────────── */

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);

    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, 320 * 20);
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &draw_buf;
    disp_drv.hor_res    = 320;
    disp_drv.ver_res    = 240;
    disp_drv.flush_cb   = flush_cb;
    lv_disp_drv_register(&disp_drv);

    UIKit_init();

    printf("== UIKitTests ==\n");

    RUN_TEST(test_uikitresult_to_string);

    printf("== UIView ==\n");
    RUN_TEST(test_uiview_create_destroy);
    RUN_TEST(test_uiview_frame);
    RUN_TEST(test_uiview_appearance);
    RUN_TEST(test_uiview_visibility_hierarchy);
    RUN_TEST(test_uiview_tag);
    RUN_TEST(test_uiview_callbacks);

    printf("== Components ==\n");
    RUN_TEST(test_uilabel);
    RUN_TEST(test_uibutton);
    RUN_TEST(test_uiswitch);
    RUN_TEST(test_uislider);
    RUN_TEST(test_uitextfield);
    RUN_TEST(test_uiimageview);
    RUN_TEST(test_uiscrollview);
    RUN_TEST(test_uiprogressview);
    RUN_TEST(test_uipagecontrol);

    printf("== Controllers ==\n");
    RUN_TEST(test_uiviewcontroller_lifecycle);
    RUN_TEST(test_uiviewcontroller_state_machine);
    RUN_TEST(test_uiviewcontroller_compat_hooks);
    RUN_TEST(test_uinavigationcontroller);

    printf("== Graphics / Layout ==\n");
    RUN_TEST(test_uitheme);
    RUN_TEST(test_uiview_animation);
    RUN_TEST(test_uistackview);
    RUN_TEST(test_api_aliases);

    printf("\nAll %d tests passed.\n", tests_run);
    return 0;
}
