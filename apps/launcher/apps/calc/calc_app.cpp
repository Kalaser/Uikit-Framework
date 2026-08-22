/**
 * @file calc_app.cpp — Calculator: display + 4x4 pad (accumulator demo).
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class CalcApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Calc", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0B1220);
        const int w = lv_disp_get_hor_res(NULL);

        m_back = launcher::make_back(this, v, m_nav);

        m_display = std::make_unique<uikit::UILabel>(&v);
        m_display->setText("0");
        m_display->setColor(0xFFFFFF);
        m_display->setAlignment(2);   /* right aligned */
        m_display->setFont(&lv_font_montserrat_48);
        m_display->setFrame(20, 130, w - 40, 70);

        static const char *kPad[4][4] = {
            { "7", "8", "9", "/" },
            { "4", "5", "6", "*" },
            { "1", "2", "3", "-" },
            { "C", "0", "=", "+" },
        };
        const int cols = 4, rows = 4;
        const int btn_w = w / cols;
        const int btn_h = 74;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                const char *key = kPad[r][c];
                auto b = std::make_unique<uikit::UIButton>(&v);
                b->setTitle(key);
                b->setTitleColor(0xFFFFFF);
                b->setBackgroundColor(0x1E293B);
                b->setCornerRadius(18);
                b->setFrame(c * btn_w + 10, 220 + r * (btn_h + 10), btn_w - 20, btn_h);
                lv_obj_t *lbl = lv_obj_get_child(::UIView_native(b->native()), 0);
                if (lbl) lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, 0);
                b->onClick([this, key](uikit::UIButton *) { press(key); });
                m_pad.push_back(std::move(b));
            }
        }
    }

    void onDestroy() override {
        launcher::vc_log("Calc", "onDestroy");
        m_pad.clear();
        m_display.reset();
        m_back.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UILabel> m_display;
    std::vector<std::unique_ptr<uikit::UIButton>> m_pad;
    int m_acc = 0;
    bool m_entered = false;

    void press(const char *key)
    {
        if (!m_display) return;
        if (key[0] == 'C') {
            m_acc = 0;
            m_entered = false;
            m_display->setText("0");
        } else if (key[0] == '=') {
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%d", m_acc);
            m_display->setText(buf);
        } else if (key[0] >= '0' && key[0] <= '9') {
            int d = key[0] - '0';
            m_acc = m_entered ? m_acc : 0;
            m_acc = m_acc * 10 + d;
            m_entered = true;
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%d", m_acc);
            m_display->setText(buf);
        } else {
            /* operator placeholder — just echo it */
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%d %s", m_acc, key);
            m_display->setText(buf);
            m_entered = false;
        }
    }
};

uikit::UIViewController *calc_app_create(uikit::UINavigationController *nav)
{
    auto *app = new CalcApp();
    app->setNav(nav);
    return app;
}
