/**
 * @file phone_app.cpp — Phone: dial pad + call button.
 *
 * Centered dial pad with tactile pressed feedback, a separated number
 * display with baseline rule, and a glowing green call button.
 */

#include "../../app_common.h"
#include "../../app_factory.h"
#include <string>

class PhoneApp : public launcher::Activity {
public:
    void onCreate() override {
        launcher::vc_log("Phone", "onCreate");
        auto v = getView();
        v.setBackgroundColor(launcher_theme::kBg);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Phone");
        m_back  = launcher::make_back(this, v, m_nav);

        /* Dialled-number display with a baseline rule */
        m_number = std::make_unique<uikit::UILabel>(&v);
        m_number->setText("");
        m_number->setColor(launcher_theme::kTextHi);
        m_number->setAlignment(1);
        m_number->setFont(launcher_theme::kFontH1);
        m_number->setFrame(40, 118, w - 80, 56);

        /* Baseline rule — NOTE: 2px height + radius 1 hit a known LVGL 8.3
         * half-circle mask hang; use a flat bar (no radius) instead. */
        m_rule = std::make_unique<uikit::UIView>(&v);
        m_rule->setBackgroundColor(launcher_theme::kSurfaceAlt);
        m_rule->setCornerRadius(0);
        m_rule->setFrame(60, 190, w - 120, 2);

        /* 4×3 dial pad, centered block of 3 columns */
        static const char kKeys[] = { '1','2','3','4','5','6','7','8','9','*','0','#' };
        const int cols = 3, rows = 4;
        const int key_w = 76, key_h = 64, gap_x = 16, gap_y = 14;
        const int pad_w = key_w * cols + gap_x * (cols - 1);
        const int x0 = (w - pad_w) / 2;
        const int pad_top = 216;
        for (int i = 0; i < 12; i++) {
            auto key = std::make_unique<uikit::UIButton>(&v);
            char t[2] = { kKeys[i], '\0' };
            key->setTitle(t);
            key->setTitleColor(launcher_theme::kTextHi);
            key->setBackgroundColor(launcher_theme::kSurface);
            key->setCornerRadius(launcher_theme::kRadiusCtl + 4);
            launcher_theme::style_pressed(*key, launcher_theme::kSurfaceAlt);
            key->setFrame(x0 + (i % cols) * (key_w + gap_x),
                          pad_top + (i / cols) * (key_h + gap_y),
                          key_w, key_h);
            lv_obj_t *lbl = lv_obj_get_child(::UIView_native(key->native()), 0);
            if (lbl) lv_obj_set_style_text_font(lbl, launcher_theme::kFontH2, 0);
            key->onClick([this, ch = kKeys[i]](uikit::UIButton *) {
                if (!m_number) return;
                const char *cur = m_number->text();
                std::string s = cur ? cur : "";
                s += ch;
                m_number->setText(s.c_str());
            });
            m_keys.push_back(std::move(key));
        }

        /* Call button: green, centered under the pad.
         * Radius 20 < h/2 — a half-pill radius (28 == h/2) trips an LVGL
         * 8.3 draw hang on partial redraws. */
        m_call = std::make_unique<uikit::UIButton>(&v);
        m_call->setTitle(LV_SYMBOL_CALL);
        m_call->setTitleColor(launcher_theme::kTextHi);
        m_call->setBackgroundColor(0x22C55E);
        m_call->setCornerRadius(20);
        launcher_theme::style_pressed(*m_call, 0x15803D);
        m_call->setFrame(w / 2 - 44, pad_top + rows * (key_h + gap_y) + 8, 88, 56);
        m_call->onClick([this](uikit::UIButton *) {
            if (!m_number) return;
            const char *n = m_number->text();
            if (n && *n) {
                std::string s = "Dialing " + std::string(n) + " …";
                m_number->setText(s.c_str());
            }
        });
    }

    void onDestroy() override {
        launcher::vc_log("Phone", "onDestroy");
        m_call.reset();
        m_keys.clear();
        m_rule.reset();
        m_number.reset();
        m_back.reset();
        m_title.reset();
    }

private:
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UILabel> m_number;
    std::unique_ptr<uikit::UIView> m_rule;
    std::vector<std::unique_ptr<uikit::UIButton>> m_keys;
    std::unique_ptr<uikit::UIButton> m_call;
};

uikit::UIViewController *phone_app_create(uikit::UINavigationController *nav)
{
    auto *app = new PhoneApp();
    app->setNav(nav);
    return app;
}
