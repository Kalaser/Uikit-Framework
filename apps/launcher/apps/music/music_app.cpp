/**
 * @file music_app.cpp — Music: track list + transport + volume.
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class MusicApp : public launcher::Activity {
public:
    void onCreate() override {
        launcher::vc_log("Music", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Music");
        m_back  = launcher::make_back(this, v, m_nav);

        /* Track list */
        static const char *kTracks[] = { "01  Synthwave Sky", "02  Midnight Drive", "03  Neon Rain" };
        for (int i = 0; i < 3; i++) {
            auto row = std::make_unique<uikit::UIButton>(&v);
            row->setTitle(kTracks[i]);
            row->setTitleColor(0xE2E8F0);
            row->setBackgroundColor(0x1E293B);
            row->setCornerRadius(12);
            row->setFrame(20, 110 + i * 60, w - 40, 50);
            lv_obj_t *lbl = lv_obj_get_child(::UIView_native(row->native()), 0);
            if (lbl) lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, 0);
            row->onClick([this, i](uikit::UIButton *) {
                m_track = i;
                if (m_now) m_now->setText(kTracks[i]);
            });
            m_rows.push_back(std::move(row));
        }

        /* Now playing */
        m_now = std::make_unique<uikit::UILabel>(&v);
        m_now->setText(kTracks[0]);
        m_now->setColor(0x22D3EE);
        m_now->setAlignment(1);
        m_now->setFont(&lv_font_montserrat_16);
        m_now->setFrame(0, 300, w, 30);

        /* Transport: play/pause */
        m_play = std::make_unique<uikit::UIButton>(&v);
        m_play->setTitle(LV_SYMBOL_PLAY);
        m_play->setTitleColor(0xFFFFFF);
        m_play->setBackgroundColor(0x22D3EE);
        m_play->setCornerRadius(26);
        m_play->setFrame(w / 2 - 26, 340, 52, 52);
        m_play->onClick([this](uikit::UIButton *) {
            m_playing = !m_playing;
            m_play->setTitle(m_playing ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);
            if (m_playing) {
                m_tick_timer = lv_timer_create([](lv_timer_t *t) {
                    static_cast<MusicApp *>(t->user_data)->advance();
                }, 300, this);
            } else if (m_tick_timer) {
                lv_timer_del(m_tick_timer);
                m_tick_timer = nullptr;
            }
        });

        /* Progress bar */
        m_progress = std::make_unique<uikit::UIProgressView>(&v);
        m_progress->setRange(0, 100);
        m_progress->setValue(0);
        m_progress->setTrackColor(0x1E293B);
        m_progress->setProgressColor(0x22D3EE);
        m_progress->setFrame(20, 420, w - 40, 8);

        /* Volume slider */
        auto *vol = new uikit::UISlider(&v);
        vol->setRange(0, 100);
        vol->setValue(70);
        vol->setIndicatorColor(0x818CF8);
        vol->setFrame(20, 450, w - 40, 10);
        vol->onChange([](int v) { std::printf("[Music] volume: %d\n", v); });
        m_volume.reset(vol);

        auto *vol_label = new uikit::UILabel(&v);
        vol_label->setText("Volume");
        vol_label->setColor(0x94A3B8);
        vol_label->setFrame(20, 470, w - 40, 24);
        m_labels.push_back(std::unique_ptr<uikit::UILabel>(vol_label));
    }

    void onDestroy() override {
        launcher::vc_log("Music", "onDestroy");
        if (m_tick_timer) { lv_timer_del(m_tick_timer); m_tick_timer = nullptr; }
        m_labels.clear();
        m_volume.reset();
        m_progress.reset();
        m_play.reset();
        m_now.reset();
        m_rows.clear();
        m_back.reset();
        m_title.reset();
    }

private:
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::vector<std::unique_ptr<uikit::UIButton>> m_rows;
    std::unique_ptr<uikit::UILabel> m_now;
    std::unique_ptr<uikit::UIButton> m_play;
    std::unique_ptr<uikit::UIProgressView> m_progress;
    std::unique_ptr<uikit::UISlider> m_volume;
    std::vector<std::unique_ptr<uikit::UILabel>> m_labels;
    lv_timer_t *m_tick_timer = nullptr;
    int m_track = 0;
    bool m_playing = false;

    void advance()
    {
        if (m_progress) {
            int v = m_progress->value() + 2;
            if (v >= 100) v = 0;
            m_progress->setValue(v);
        }
    }
};

uikit::UIViewController *music_app_create(uikit::UINavigationController *nav)
{
    auto *app = new MusicApp();
    app->setNav(nav);
    return app;
}
