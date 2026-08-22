/**
 * Example: C++ basic — demonstrates the C++ wrapper
 * (README §8 / §17 usage).
 */

#include "UIKit.hpp"
#include <cstdio>

/* ── Headless display ─────────────────────────────────────────────────────── */

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
    lv_disp_draw_buf_init(&draw_buf, buf1, nullptr, 320 * 20);
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res  = 320;
    disp_drv.ver_res  = 240;
    disp_drv.flush_cb = flush_cb;
    lv_disp_drv_register(&disp_drv);
}

/* ── MainViewController ───────────────────────────────────────────────────── */

class MainViewController : public uikit::UIViewController {
public:
    void onCreate() override {
        auto view = getView();
        view.setBackgroundColor(0xF2F2F7);

        auto title = uikit::UILabel::create();
        title.setText("UIKit C++ Demo");
        title.setColor(0x000000);
        title.setAlignment(1);
        title.setFrame(0, 20, 320, 30);
        view.addSubview(title);

        auto button = uikit::UIButton::create();
        button.setTitle("Start");
        button.setTitleColor(0xFFFFFF);
        button.setFrame(40, 80, 240, 50);
        button.onClick([](uikit::UIButton *) { std::printf("Start clicked\n"); });
        view.addSubview(button);

        auto slider = uikit::UISlider::create();
        slider.setRange(0, 100);
        slider.setValue(50);
        slider.onChange([](int v) { std::printf("slider: %d\n", v); });
        slider.setFrame(40, 150, 240, 10);
        view.addSubview(slider);
    }
};

/* ── Main ─────────────────────────────────────────────────────────────────── */

int main(void)
{
    lv_init();
    setup_display();
    ::UIKit_init();

    auto *main_vc = new MainViewController();
    uikit::UINavigationController navigationController(main_vc);

    for (int i = 0; i < 20; i++) {
        lv_tick_inc(5);
        lv_timer_handler();
        ::UIKit_process();
    }

    std::printf("C++ example finished.\n");
    return 0;
}
