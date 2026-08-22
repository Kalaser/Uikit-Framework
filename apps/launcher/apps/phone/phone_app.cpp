/**
 * @file phone_app.cpp — Phone: dial pad + call button.
 */

#include "../../app_common.h"
#include "../../app_factory.h"
#include <string>

class PhoneApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Phone", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Phone");
        m_back  = launcher::make_back(this, v, m_nav);

        /* Dialled-number display */
        m_number = std::make_unique<uikit::UILabel>(&v);
        m_number->setText("");
        m_number->setColor(0xFFFFFF);
        m_number->setAlignment(1);
        m_number->setFont(&lv_font_montserrat_28);
        m_number->setFrame(0, 130, w, 50);

        /* 4x3 dial pad */
        static const char kKeys[] = { '1','2','3','4','5','6','7','8','9','*','0','#' };
        const int cols = 3, rows = 4;
        const int key_w = w / cols;
        const int key_h = 70;
        const int pad_top = 200;
        for (int i = 0; i < 12; i++) {
            auto key = std::make_unique<uikit::UIButton>(&v);
            char t[2] = { kKeys[i], '\0' };
            key->setTitle(t);
            key->setTitleColor(0xFFFFFF);
            key->setBackgroundColor(0x1E293B);
            key->setCornerRadius(24);
            key->setFrame((i % cols) * key_w + 20, pad_top + (i / cols) * (key_h + 10),
                          key_w - 40, key_h);
            lv_obj_t *lbl = lv_obj_get_child(::UIView_native(key->native()), 0);
            if (lbl) lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, 0);
            key->onClick([this, ch = kKeys[i]](uikit::UIButton *) {
                if (m_number) {
                    const char *cur = m_number->text();
                    std::string s = cur ? cur : "";
                    s += ch;
                    m_number->setText(s.c_str());
                }
            });
            m_keys.push_back(std::move(key));
        }

        /* Call button */
        m_call = std::make_unique<uikit::UIButton>(&v);
        m_call->setTitle(LV_SYMBOL_CALL);
        m_call->setTitleColor(0xFFFFFF);
        m_call->setBackgroundColor(0x22C55E);
        m_call->setCornerRadius(28);
        m_call->setFrame(w / 2 - 44, pad_top + 4 * (key_h + 10), 88, 56);
        m_call->onClick([this](uikit::UIButton *) {
            if (m_number) {
                const char *n = m_number->text();
                if (n && *n) {
                    std::string s = "Dialing " + std::string(n) + " …";
                    m_number->setText(s.c_str());
                }
            }
        });
    }

    void onDestroy() override {
        launcher::vc_log("Phone", "onDestroy");
        m_call.reset();
        m_keys.clear();
        m_number.reset();
        m_back.reset();
        m_title.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UILabel> m_number;
    std::vector<std::unique_ptr<uikit::UIButton>> m_keys;
    std::unique_ptr<uikit::UIButton> m_call;
};

uikit::UIViewController *phone_app_create(uikit::UINavigationController *nav)
{
    auto *app = new PhoneApp();
    app->setNav(nav);
    return app;
}
