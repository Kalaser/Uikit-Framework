/**
 * @file clock_app.cpp — Clock: live time + stopwatch.
 */

#include "../../app_common.h"
#include "../../app_factory.h"
#include <ctime>

class ClockApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Clock", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Clock");
        m_back  = launcher::make_back(this, v, m_nav);

        m_time = std::make_unique<uikit::UILabel>(&v);
        m_time->setColor(0xFFFFFF);
        m_time->setAlignment(1);
        m_time->setFont(&lv_font_montserrat_48);
        m_time->setFrame(0, 170, w, 90);

        m_date = std::make_unique<uikit::UILabel>(&v);
        m_date->setColor(0x94A3B8);
        m_date->setAlignment(1);
        m_date->setFrame(0, 270, w, 30);

        m_stopwatch = std::make_unique<uikit::UILabel>(&v);
        m_stopwatch->setText("00:00.0");
        m_stopwatch->setColor(0x22D3EE);
        m_stopwatch->setAlignment(1);
        m_stopwatch->setFont(&lv_font_montserrat_28);
        m_stopwatch->setFrame(0, 340, w, 50);

        m_start = std::make_unique<uikit::UIButton>(&v);
        m_start->setTitle(LV_SYMBOL_PLAY);
        m_start->setTitleColor(0xFFFFFF);
        m_start->setBackgroundColor(0x22C55E);
        m_start->setCornerRadius(24);
        m_start->setFrame(w / 2 - 90, 420, 80, 48);
        m_start->onClick([this](uikit::UIButton *) {
            m_running = !m_running;
            m_start->setTitle(m_running ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);
        });

        m_reset = std::make_unique<uikit::UIButton>(&v);
        m_reset->setTitle(LV_SYMBOL_REFRESH);
        m_reset->setTitleColor(0xFFFFFF);
        m_reset->setBackgroundColor(0x475569);
        m_reset->setCornerRadius(24);
        m_reset->setFrame(w / 2 + 10, 420, 80, 48);
        m_reset->onClick([this](uikit::UIButton *) { m_sw_ms = 0; update_sw(); });

        /* 100 ms ticker */
        m_timer = lv_timer_create([](lv_timer_t *t) {
            static_cast<ClockApp *>(t->user_data)->tick();
        }, 100, this);
        update_clock();
    }

    void onDestroy() override {
        launcher::vc_log("Clock", "onDestroy");
        if (m_timer) { lv_timer_del(m_timer); m_timer = nullptr; }
        m_reset.reset();
        m_start.reset();
        m_stopwatch.reset();
        m_date.reset();
        m_time.reset();
        m_back.reset();
        m_title.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UILabel> m_time;
    std::unique_ptr<uikit::UILabel> m_date;
    std::unique_ptr<uikit::UILabel> m_stopwatch;
    std::unique_ptr<uikit::UIButton> m_start;
    std::unique_ptr<uikit::UIButton> m_reset;
    lv_timer_t *m_timer = nullptr;
    bool m_running = false;
    int m_sw_ms = 0;

    void update_clock()
    {
        if (!m_time) return;
        std::time_t now = std::time(nullptr);
        std::tm *tm = std::localtime(&now);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%H:%M:%S", tm);
        m_time->setText(buf);
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %a", tm);
        m_date->setText(buf);
    }

    void update_sw()
    {
        if (!m_stopwatch) return;
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%02d:%02d.%d",
                      m_sw_ms / 60000, (m_sw_ms / 1000) % 60, (m_sw_ms / 100) % 10);
        m_stopwatch->setText(buf);
    }

    void tick()
    {
        update_clock();
        if (m_running) {
            m_sw_ms = (m_sw_ms + 100) % 600000;
            update_sw();
        }
    }
};

uikit::UIViewController *clock_app_create(uikit::UINavigationController *nav)
{
    auto *app = new ClockApp();
    app->setNav(nav);
    return app;
}
