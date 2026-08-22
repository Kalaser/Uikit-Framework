/**
 * @file video_app.cpp — Video: player surface + transport.
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class VideoApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Video", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0B1220);
        const int w = lv_disp_get_hor_res(NULL);
        const int h = lv_disp_get_ver_res(NULL);

        m_back = launcher::make_back(this, v, m_nav);

        /* Player surface (16:9 approx) */
        m_surface = std::make_unique<uikit::UIView>(&v);
        m_surface->setFrame(0, 110, w, (int)(w * 9 / 16));
        m_surface->setBackgroundColor(0x000000);

        m_title = std::make_unique<uikit::UILabel>(&v);
        m_title->setText(LV_SYMBOL_VIDEO "  Demo clip");
        m_title->setColor(0xFFFFFF);
        m_title->setAlignment(1);
        m_title->setFont(&lv_font_montserrat_16);
        m_title->setFrame(0, 110 + (int)(w * 9 / 16) / 2 - 20, w, 40);

        m_play = std::make_unique<uikit::UIButton>(&v);
        m_play->setTitle(LV_SYMBOL_PLAY);
        m_play->setTitleColor(0xFFFFFF);
        m_play->setBackgroundColor(0x22D3EE);
        m_play->setCornerRadius(22);
        m_play->setFrame(w / 2 - 22, 110 + (int)(w * 9 / 16) / 2 + 20, 44, 44);
        m_play->onClick([this](uikit::UIButton *) {
            m_playing = !m_playing;
            m_play->setTitle(m_playing ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);
            if (m_playing) {
                m_tick = lv_timer_create([](lv_timer_t *t) {
                    static_cast<VideoApp *>(t->user_data)->advance();
                }, 300, this);
            } else if (m_tick) {
                lv_timer_del(m_tick);
                m_tick = nullptr;
            }
        });

        m_progress = std::make_unique<uikit::UIProgressView>(&v);
        m_progress->setRange(0, 100);
        m_progress->setValue(0);
        m_progress->setTrackColor(0x1E293B);
        m_progress->setProgressColor(0x22D3EE);
        m_progress->setFrame(20, 110 + (int)(w * 9 / 16) + 30, w - 40, 8);

        m_time = std::make_unique<uikit::UILabel>(&v);
        m_time->setText("00:00 / 03:20");
        m_time->setColor(0x94A3B8);
        m_time->setAlignment(1);
        m_time->setFrame(0, 110 + (int)(w * 9 / 16) + 46, w, 26);
    }

    void onDestroy() override {
        launcher::vc_log("Video", "onDestroy");
        if (m_tick) { lv_timer_del(m_tick); m_tick = nullptr; }
        m_time.reset();
        m_progress.reset();
        m_play.reset();
        m_title.reset();
        m_surface.reset();
        m_back.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UIView> m_surface;
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_play;
    std::unique_ptr<uikit::UIProgressView> m_progress;
    std::unique_ptr<uikit::UILabel> m_time;
    lv_timer_t *m_tick = nullptr;
    bool m_playing = false;

    void advance()
    {
        int v = m_progress->value() + 3;
        if (v > 100) {
            v = 0;
            m_playing = false;
            m_play->setTitle(LV_SYMBOL_PLAY);
            if (m_tick) { lv_timer_del(m_tick); m_tick = nullptr; }
        }
        m_progress->setValue(v);
        if (m_time) {
            char buf[24];
            std::snprintf(buf, sizeof(buf), "%02d:%02d / 03:20",
                          (v * 200) / 100 / 60, (v * 200) / 100 % 60);
            m_time->setText(buf);
        }
    }
};

uikit::UIViewController *video_app_create(uikit::UINavigationController *nav)
{
    auto *app = new VideoApp();
    app->setNav(nav);
    return app;
}
