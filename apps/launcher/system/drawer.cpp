/**
 * @file drawer.cpp — App drawer (Android): every installed app grouped by
 *       category; tapping an entry launches it.
 */

#include "../app_common.h"
#include "../app_factory.h"
#include <cstring>

class DrawerViewController : public launcher::Activity {
public:
    void onCreate() override {
        launcher::vc_log("Drawer", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "All Apps");
        m_back  = launcher::make_back(this, v, m_nav);

        m_scroll = std::make_unique<uikit::UIScrollView>(&v);
        m_scroll->setFrame(0, 90, w, lv_disp_get_ver_res(NULL) - 90);
        m_scroll->setVerticalScroll(true);
        m_scroll->setHorizontalScroll(false);
        m_scroll->setScrollbarVisible(false);
        m_scroll->setBackgroundColor(0x0F172A);
        m_scroll->setCornerRadius(0);
        lv_obj_set_style_border_width(::UIView_native(m_scroll->native()), 0, 0);

        /* Grouped by category */
        static const char *kGroups[] = { "comms", "media", "tools", "system" };
        int y = 0;
        for (int g = 0; g < 4; g++) {
            bool first = true;
            for (int i = 0; i < 13; i++) {
                const AppDescriptor &app = kLauncherApps[i];
                if (std::strcmp(app.category, kGroups[g]) != 0) continue;
                if (first) {
                    auto *hdr = new uikit::UILabel(&*m_scroll);
                    hdr->setText(kGroups[g]);
                    hdr->setColor(0x94A3B8);
                    hdr->setFont(&lv_font_montserrat_14);
                    hdr->setFrame(24, y, w - 48, 28);
                    m_labels.push_back(std::unique_ptr<uikit::UILabel>(hdr));
                    y += 34;
                    first = false;
                }
                auto row = std::make_unique<uikit::UIButton>(&*m_scroll);
                char title[96];
                std::snprintf(title, sizeof(title), "%s   %s", app.icon, app.name);
                row->setTitle(title);
                row->setTitleColor(0xE2E8F0);
                row->setBackgroundColor(0x1E293B);
                row->setCornerRadius(12);
                row->setFrame(20, y, w - 40, 52);
                lv_obj_t *lbl = lv_obj_get_child(::UIView_native(row->native()), 0);
                if (lbl) {
                    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_16, 0);
                    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, 0);
                }
                row->onClick([this, &app](uikit::UIButton *) {
                    if (m_nav) startActivity(launcher::Intent(app.id));
                });
                m_rows.push_back(std::move(row));
                y += 60;
            }
        }
    }

    void onDestroy() override {
        launcher::vc_log("Drawer", "onDestroy");
        m_rows.clear();
        m_labels.clear();
        m_scroll.reset();
        m_back.reset();
        m_title.reset();
    }

private:
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UIScrollView> m_scroll;
    std::vector<std::unique_ptr<uikit::UIButton>> m_rows;
    std::vector<std::unique_ptr<uikit::UILabel>> m_labels;
};

uikit::UIViewController *launcher_drawer_create(uikit::UINavigationController *nav)
{
    auto *vc = new DrawerViewController();
    vc->setNav(nav);
    return vc;
}
