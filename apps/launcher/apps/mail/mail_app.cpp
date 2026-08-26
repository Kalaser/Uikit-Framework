/**
 * @file mail_app.cpp — Mail: inbox list; tap to read.
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class MailApp : public launcher::Activity {
public:
    void onCreate() override {
        launcher::vc_log("Mail", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Inbox");
        m_back  = launcher::make_back(this, v, m_nav);

        static const struct { const char *from; const char *subj; } kMail[] = {
            { "Alice",   "Meeting at 3pm" },
            { "Bob",     "Build passed" },
            { "CI Bot",  "Test failed on edge" },
            { "HR",      "Payroll available" },
        };
        for (int i = 0; i < 4; i++) {
            auto row = std::make_unique<uikit::UIButton>(&v);
            row->setBackgroundColor(0x1E293B);
            row->setCornerRadius(12);
            row->setFrame(20, 110 + i * 76, w - 40, 66);
            char title[96];
            std::snprintf(title, sizeof(title), "%s\n%s", kMail[i].from, kMail[i].subj);
            row->setTitle(title);
            row->setTitleColor(0xE2E8F0);
            lv_obj_t *lbl = lv_obj_get_child(::UIView_native(row->native()), 0);
            if (lbl) {
                lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
                lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, 0);
                lv_obj_set_style_text_color(lbl, lv_color_hex(0x7DD3FC), LV_STATE_DEFAULT);
            }
            row->onClick([this, i](uikit::UIButton *) {
                if (m_body) {
                    m_body->setText(kMail[i].subj);
                    m_body->setText("You have one new message.\nTap the row to dismiss.");
                }
            });
            m_rows.push_back(std::move(row));
        }

        m_body = std::make_unique<uikit::UILabel>(&v);
        m_body->setText("Select a message");
        m_body->setColor(0x94A3B8);
        m_body->setAlignment(1);
        m_body->setFrame(20, 440, w - 40, 60);
    }

    void onDestroy() override {
        launcher::vc_log("Mail", "onDestroy");
        m_body.reset();
        m_rows.clear();
        m_back.reset();
        m_title.reset();
    }

private:
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::vector<std::unique_ptr<uikit::UIButton>> m_rows;
    std::unique_ptr<uikit::UILabel> m_body;
};

uikit::UIViewController *mail_app_create(uikit::UINavigationController *nav)
{
    auto *app = new MailApp();
    app->setNav(nav);
    return app;
}
