/**
 * @file recents.cpp — Recents (Android Recents / iOS app switcher):
 *       re-launch previously opened apps.
 */

#include "../app_common.h"
#include "../app_factory.h"
#include <vector>

class RecentsViewController : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }
    void setRecents(const std::vector<const AppDescriptor *> &recents) { m_recents = recents; }

    void onCreate() override {
        launcher::vc_log("Recents", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Recents");
        m_back  = launcher::make_back(this, v, m_nav);

        if (m_recents.empty()) {
            auto *empty = new uikit::UILabel(&v);
            empty->setText("No recent apps");
            empty->setColor(0x64748B);
            empty->setAlignment(1);
            empty->setFrame(0, 260, w, 40);
            m_labels.push_back(std::unique_ptr<uikit::UILabel>(empty));
            return;
        }

        m_scroll = std::make_unique<uikit::UIScrollView>(&v);
        m_scroll->setFrame(0, 90, w, lv_disp_get_ver_res(NULL) - 90);
        m_scroll->setVerticalScroll(true);
        m_scroll->setHorizontalScroll(false);
        m_scroll->setScrollbarVisible(false);
        m_scroll->setBackgroundColor(0x0F172A);
        m_scroll->setCornerRadius(0);
        lv_obj_set_style_border_width(::UIView_native(m_scroll->native()), 0, 0);

        int y = 12;
        for (const AppDescriptor *app : m_recents) {
            auto card = std::make_unique<uikit::UIButton>(&*m_scroll);
            char title[96];
            std::snprintf(title, sizeof(title), "%s   %s", app->icon, app->name);
            card->setTitle(title);
            card->setTitleColor(0xFFFFFF);
            card->setBackgroundColor(app->color);
            card->setCornerRadius(16);
            card->setFrame(20, y, w - 40, 64);
            lv_obj_t *lbl = lv_obj_get_child(::UIView_native(card->native()), 0);
            if (lbl) {
                lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, 0);
                lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, 0);
            }
            card->onClick([this, app](uikit::UIButton *) {
                if (m_nav) m_nav->push(app->create(m_nav));
            });
            m_cards.push_back(std::move(card));
            y += 76;
        }
    }

    void onDestroy() override {
        launcher::vc_log("Recents", "onDestroy");
        m_cards.clear();
        m_labels.clear();
        m_scroll.reset();
        m_back.reset();
        m_title.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::vector<const AppDescriptor *> m_recents;
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UIScrollView> m_scroll;
    std::vector<std::unique_ptr<uikit::UIButton>> m_cards;
    std::vector<std::unique_ptr<uikit::UILabel>> m_labels;
};

uikit::UIViewController *launcher_recents_create(
    uikit::UINavigationController *nav,
    const std::vector<const AppDescriptor *> &recents)
{
    auto *vc = new RecentsViewController();
    vc->setNav(nav);
    vc->setRecents(recents);
    return vc;
}
