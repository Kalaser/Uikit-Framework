/**
 * UIKitCppTests — smoke tests for the C++ wrapper (uikit/cpp/UIKit.hpp).
 */

#include "UIKit.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>

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

#define RUN_TEST(fn)                                  \
    do {                                              \
        tests_run++;                                  \
        std::printf("  [RUN ] %s\n", #fn);            \
        fn();                                         \
        std::printf("  [ OK ] %s\n", #fn);            \
    } while (0)

/* ── Test controller ──────────────────────────────────────────────────────── */

struct TestController : public uikit::UIViewController {
    int *destroyed;   /* points to a live counter outside the object */

    explicit TestController(int *d) : destroyed(d) {}

    void onCreate() override { setTitle("Test"); auto v = getView(); v.setBackgroundColor(0xFFFFFF); }
    void onDestroy() override { if (destroyed) *destroyed += 1; }
};

/* ── Lifecycle virtual dispatch + nav ownership ───────────────────────────── */

static void test_nav_ownership_and_destroy(void)
{
    int destroyed = 0;
    {
        TestController *root = new TestController(&destroyed);
        uikit::UINavigationController nav(root);

        assert(nav.count() == 1);
        assert(nav.top() == root);
        assert(root->state() == ::UIViewControllerState_Resumed);
        assert(std::strcmp(root->title(), "Test") == 0);
    }
    /* nav destructor: C layer fires onDestroy (virtual dispatch) then deletes */
    assert(destroyed == 1);
}

static void test_push_pop_ownership(void)
{
    int root_destroyed = 0, second_destroyed = 0, third_destroyed = 0;
    {
        TestController *root   = new TestController(&root_destroyed);
        TestController *second = new TestController(&second_destroyed);
        TestController *third  = new TestController(&third_destroyed);

        uikit::UINavigationController nav(root);
        nav.push(second);
        nav.push(third);
        assert(nav.count() == 3);
        assert(nav.at(1) == second);

        /* pop: ownership returns to the caller */
        uikit::UIViewController *popped = nav.pop();
        assert(popped == third);
        assert(nav.count() == 2);
        assert(popped->state() == ::UIViewControllerState_Stopped);
        /* Explicit destroy() while alive → virtual dispatch to TestController,
         * then delete the C++ object (destructor skips: C struct freed). */
        popped->destroy();
        delete popped;
        assert(third_destroyed == 1);

        /* popToRoot: returns the popped controllers (top first) */
        auto popped_vec = nav.popToRoot();
        assert(popped_vec.size() == 1);
        assert(popped_vec[0] == second);
        assert(nav.count() == 1);
        for (auto *vc : popped_vec) {
            vc->destroy();
            delete vc;
        }
        assert(second_destroyed == 1);
    }
    /* nav destroys the root controller */
    assert(root_destroyed == 1);
}

static void test_controller_move(void)
{
    int destroyed = 0;
    {
        uikit::UIViewController a;
        /* give it a C object first */
        ::UIViewController *raw = a.c_ptr();
        assert(raw != nullptr);

        uikit::UIViewController b = std::move(a);
        assert(a.c_ptr() == nullptr);       /* source inert */
        assert(b.c_ptr() == raw);           /* target owns the C struct */
        assert(raw->user_data == &b);       /* user_data re-bound to target */

        ::UIViewController_onCreate(b.c_ptr());
        assert(b.state() == ::UIViewControllerState_Created);
    }
    (void)destroyed;
}

/* ── Components: events & RAII ────────────────────────────────────────────── */

static void test_button_on_click(void)
{
    uikit::UIButton button;
    button.setTitle("OK");
    button.setFrame(0, 0, 100, 40);
    assert(std::strcmp(button.title(), "OK") == 0);

    int clicks = 0;
    button.onClick([&clicks](uikit::UIButton *) { clicks++; });

    lv_event_send(::UIView_native(button.native()), LV_EVENT_SHORT_CLICKED, nullptr);
    assert(clicks == 1);
    lv_event_send(::UIView_native(button.native()), LV_EVENT_SHORT_CLICKED, nullptr);
    assert(clicks == 2);
}

static void test_button_move_rebinds_events(void)
{
    uikit::UIButton a;
    int clicks = 0;
    a.onClick([&clicks](uikit::UIButton *) { clicks++; });

    uikit::UIButton b = std::move(a);   /* event user_data must rebind to b */
    assert(a.native() == nullptr);

    lv_event_send(::UIView_native(b.native()), LV_EVENT_SHORT_CLICKED, nullptr);
    assert(clicks == 1);
}

static void test_switch_and_slider(void)
{
    uikit::UISwitch sw;
    sw.setOn(true);
    assert(sw.isOn());
    sw.setOnInstant(false);
    assert(!sw.isOn());

    int switch_changes = 0;
    sw.onChange([&switch_changes](bool on) { switch_changes += on ? 1 : 0; });
    sw.setOn(true);
    lv_event_send(::UIView_native(sw.native()), LV_EVENT_VALUE_CHANGED, nullptr);
    assert(switch_changes == 1);

    uikit::UISlider slider;
    slider.setRange(0, 100);
    slider.setValue(42);
    assert(slider.value() == 42);

    int slider_value = -1;
    slider.onChange([&slider_value](int v) { slider_value = v; });
    lv_event_send(::UIView_native(slider.native()), LV_EVENT_VALUE_CHANGED, nullptr);
    assert(slider_value == 42);
}

static void test_view_raii_and_frame(void)
{
    /* Owning wrapper frees the C object on destruction */
    ::UIView *raw = ::UIView_create(nullptr);
    {
        uikit::UIView v(raw, true);
        v.setFrame(10, 20, 100, 50);
        int32_t x = 0, y = 0, w = 0, h = 0;
        v.getFrame(&x, &y, &w, &h);
        assert(x == 10 && y == 20 && w == 100 && h == 50);
        assert(v.width() == 100 && v.height() == 50);
        v.setBackgroundColor(0xFF0000);
        v.setHidden(true);
        assert(v.isHidden());
    }
    /* Borrowed wrapper does NOT free */
    ::UIView *kept = ::UIView_create(nullptr);
    {
        uikit::UIView v(kept, false);
        v.setSize(10, 10);
    }
    ::UIView_set_frame(kept, 1, 2, 3, 4);   /* still alive */
    ::UIView_destroy(kept);
}

static void test_controller_get_view_borrowed(void)
{
    TestController *tc = new TestController(nullptr);
    uikit::UINavigationController nav(tc);

    /* borrowed wrapper: must not free the view owned by the controller */
    {
        uikit::UIView v = tc->getView();
        assert(v.valid());
        v.setFrame(0, 0, 320, 240);
        uikit::UILabel label;
        v.addSubview(label);   /* child owned by its own wrapper */
    }
    /* nav destroys controller + view; no double free */
    (void)nav;
}

static void test_stackview_and_textfield(void)
{
    uikit::UIStackView stack;
    stack.setAxis(::UIStackAxis_VERTICAL);
    stack.setSpacing(8);
    stack.setPadding(4);

    uikit::UILabel a;
    uikit::UILabel b;
    stack.addArrangedSubview(a);
    stack.addArrangedSubview(b);
    assert(a.valid() && b.valid());

    uikit::UITextField field;
    field.setText("abc");
    assert(std::strcmp(field.text(), "abc") == 0);
    field.setPlaceholder("hint");
    field.setPasswordMode(true);
    field.setPasswordMode(false);

    std::string last;
    field.onChange([&last](const char *t) { last = t ? t : ""; });
    lv_event_send(::UIView_native(field.native()), LV_EVENT_VALUE_CHANGED, nullptr);
    assert(last == "abc");
}

static void test_pagecontrol_progress(void)
{
    uikit::UIPageControl pages;
    pages.setPageCount(3);
    assert(pages.pageCount() == 3);
    pages.setCurrentPage(2);
    assert(pages.currentPage() == 2);

    uikit::UIProgressView progress;
    progress.setRange(0, 100);
    progress.setValueInstant(60);
    assert(progress.value() == 60);
    progress.setProgressColor(0x34C759);
}

/* ── Main ──────────────────────────────────────────────────────────────────── */

int main(void)
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, nullptr, 320 * 20);
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res  = 320;
    disp_drv.ver_res  = 240;
    disp_drv.flush_cb = flush_cb;
    lv_disp_drv_register(&disp_drv);

    ::UIKit_init();

    std::printf("== UIKitCppTests ==\n");

    RUN_TEST(test_nav_ownership_and_destroy);
    RUN_TEST(test_push_pop_ownership);
    RUN_TEST(test_controller_move);
    RUN_TEST(test_button_on_click);
    RUN_TEST(test_button_move_rebinds_events);
    RUN_TEST(test_switch_and_slider);
    RUN_TEST(test_view_raii_and_frame);
    RUN_TEST(test_controller_get_view_borrowed);
    RUN_TEST(test_stackview_and_textfield);
    RUN_TEST(test_pagecontrol_progress);

    std::printf("\nAll %d C++ tests passed.\n", tests_run);
    return 0;
}
