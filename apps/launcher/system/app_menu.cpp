/**
 * @file app_menu.cpp — Long-press app menu (Android-style context menu):
 *       shows app info and offers Open / Dismiss.
 */

#include "../app_common.h"
#include "../app_factory.h"

class AppMenuViewController : public launcher::Activity {
public:
    void setApp(const AppDescriptor &app) { m_app = &app; }

    void onCreate() override {
        launcher::vc_log("AppMenu", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0B1220);
        const int w = lv_disp_get_hor_res(NULL);

        /* Dim backdrop handled by the page background; draw the card */
        m_card = std::make_unique<uikit::UIView>(&v);
        m_card->setFrame(40, 220, w - 80, 280);
        m_card->setBackgroundColor(0x1E293B);
        m_card->setCornerRadius(20);

        m_icon = std::make_unique<uikit::UILabel>(&v);
        m_icon->setText(m_app->icon);
        m_icon->setColor(0xFFFFFF);
        m_icon->setAlignment(1);
        m_icon->setFont(&lv_font_montserrat_48);
        m_icon->setFrame(40, 250, w - 80, 70);

        m_name = std::make_unique<uikit::UILabel>(&v);
        m_name->setText(m_app->name);
        m_name->setColor(0xFFFFFF);
        m_name->setAlignment(1);
        m_name->setFont(&lv_font_montserrat_20);
        m_name->setFrame(40, 330, w - 80, 34);

        char meta[96];
        std::snprintf(meta, sizeof(meta), "id: %s  ·  %s",
                      m_app->id, m_app->category);
        m_meta = std::make_unique<uikit::UILabel>(&v);
        m_meta->setText(meta);
        m_meta->setColor(0x94A3B8);
        m_meta->setAlignment(1);
        m_meta->setFrame(40, 368, w - 80, 26);

        m_open = std::make_unique<uikit::UIButton>(&v);
        m_open->setTitle("Open");
        m_open->setTitleColor(0xFFFFFF);
        m_open->setBackgroundColor(0x22D3EE);
        m_open->setCornerRadius(18);
        m_open->setFrame(w / 2 - 110, 440, 100, 40);
        m_open->onClick([this](uikit::UIButton *) {
            if (m_nav && m_app) {
                uikit::UIViewController *popped = finish();
                launcher::schedule_delete(popped);
                startActivity(launcher::Intent(m_app->id));
            }
        });

        m_close = std::make_unique<uikit::UIButton>(&v);
        m_close->setTitle("Dismiss");
        m_close->setTitleColor(0xFFFFFF);
        m_close->setBackgroundColor(0x475569);
        m_close->setCornerRadius(18);
        m_close->setFrame(w / 2 + 10, 440, 100, 40);
        m_close->onClick([this](uikit::UIButton *) {
            if (m_nav) {
                launcher::schedule_delete(finish());
            }
        });
    }

    void onDestroy() override {
        launcher::vc_log("AppMenu", "onDestroy");
        m_close.reset();
        m_open.reset();
        m_meta.reset();
        m_name.reset();
        m_icon.reset();
        m_card.reset();
    }

private:
    const AppDescriptor *m_app = nullptr;
    std::unique_ptr<uikit::UIView> m_card;
    std::unique_ptr<uikit::UILabel> m_icon;
    std::unique_ptr<uikit::UILabel> m_name;
    std::unique_ptr<uikit::UILabel> m_meta;
    std::unique_ptr<uikit::UIButton> m_open;
    std::unique_ptr<uikit::UIButton> m_close;
};

uikit::UIViewController *launcher_app_menu_create(uikit::UINavigationController *nav,
                                                  const AppDescriptor &app)
{
    auto *vc = new AppMenuViewController();
    vc->setNav(nav);
    vc->setApp(app);
    return vc;
}
