/**
 * @file store_app.cpp — Store: app cards with install progress.
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class StoreApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Store", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "App Store");
        m_back  = launcher::make_back(this, v, m_nav);

        struct Card { const char *name; const char *icon; uint32_t color; };
        static const Card kCards[] = {
            { "Weather Pro", LV_SYMBOL_TINT, 0x38BDF8 },
            { "Photo Editor", LV_SYMBOL_IMAGE, 0xF472B6 },
        };
        for (int i = 0; i < 2; i++) {
            auto card = std::make_unique<uikit::UIButton>(&v);
            card->setBackgroundColor(0x1E293B);
            card->setCornerRadius(16);
            card->setFrame(20, 110 + i * 150, w - 40, 140);
            char title[96];
            std::snprintf(title, sizeof(title), "%s %s\n%s",
                          kCards[i].icon, kCards[i].name,
                          i == 0 ? "Weather forecasts" : "Retouch photos");
            card->setTitle(title);
            card->setTitleColor(0xE2E8F0);
            m_cards.push_back(std::move(card));

            auto install = std::make_unique<uikit::UIButton>(&v);
            install->setTitle("Install");
            install->setTitleColor(0xFFFFFF);
            install->setBackgroundColor(0x22D3EE);
            install->setCornerRadius(18);
            install->setFrame(w - 130, 118 + i * 150, 96, 40);
            install->onClick([this, i](uikit::UIButton *b) {
                m_installing[i] = true;
                b->setTitle("Installing…");
                b->setEnabled(false);
                if (!m_tick) {
                    m_tick = lv_timer_create([](lv_timer_t *t) {
                        static_cast<StoreApp *>(t->user_data)->advance();
                    }, 400, this);
                }
            });
            m_install_btns.push_back(std::move(install));
        }

        m_progress = std::make_unique<uikit::UIProgressView>(&v);
        m_progress->setRange(0, 100);
        m_progress->setValue(0);
        m_progress->setTrackColor(0x1E293B);
        m_progress->setProgressColor(0x22D3EE);
        m_progress->setFrame(20, 420, w - 40, 8);
    }

    void onDestroy() override {
        launcher::vc_log("Store", "onDestroy");
        if (m_tick) { lv_timer_del(m_tick); m_tick = nullptr; }
        m_progress.reset();
        m_install_btns.clear();
        m_cards.clear();
        m_back.reset();
        m_title.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::vector<std::unique_ptr<uikit::UIButton>> m_cards;
    std::vector<std::unique_ptr<uikit::UIButton>> m_install_btns;
    std::unique_ptr<uikit::UIProgressView> m_progress;
    lv_timer_t *m_tick = nullptr;
    bool m_installing[2] = { false, false };

    void advance()
    {
        bool any = false;
        for (int i = 0; i < 2; i++) {
            if (m_installing[i]) any = true;
        }
        if (!any) {
            if (m_tick) { lv_timer_del(m_tick); m_tick = nullptr; }
            return;
        }
        int v = m_progress->value() + 8;
        if (v >= 100) {
            v = 0;
            for (int i = 0; i < 2; i++) m_installing[i] = false;
            if (m_install_btns[0]) m_install_btns[0]->setTitle("Install");
            if (m_install_btns[1]) m_install_btns[1]->setTitle("Install");
            if (m_install_btns[0]) m_install_btns[0]->setEnabled(true);
            if (m_install_btns[1]) m_install_btns[1]->setEnabled(true);
        }
        m_progress->setValue(v);
    }
};

uikit::UIViewController *store_app_create(uikit::UINavigationController *nav)
{
    auto *app = new StoreApp();
    app->setNav(nav);
    return app;
}
