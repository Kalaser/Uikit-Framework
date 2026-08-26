/**
 * @file weather_app.cpp — Weather: big temperature + stats bars.
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class WeatherApp : public launcher::Activity {
public:
    void onCreate() override {
        launcher::vc_log("Weather", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x1E3A5F);
        const int w = lv_disp_get_hor_res(NULL);

        m_back = launcher::make_back(this, v, m_nav);

        m_icon = std::make_unique<uikit::UILabel>(&v);
        m_icon->setText(LV_SYMBOL_TINT);
        m_icon->setColor(0x7DD3FC);
        m_icon->setAlignment(1);
        m_icon->setFont(&lv_font_montserrat_48);
        m_icon->setFrame(0, 140, w, 80);

        m_temp = std::make_unique<uikit::UILabel>(&v);
        m_temp->setText("26°C");
        m_temp->setColor(0xFFFFFF);
        m_temp->setAlignment(1);
        m_temp->setFont(&lv_font_montserrat_48);
        m_temp->setFrame(0, 230, w, 70);

        m_desc = std::make_unique<uikit::UILabel>(&v);
        m_desc->setText("Partly cloudy · Shenzhen");
        m_desc->setColor(0xBFDBFE);
        m_desc->setAlignment(1);
        m_desc->setFrame(0, 310, w, 30);

        auto bar = [&](const char *name, int val, uint32_t color, int y) {
            auto *lbl = new uikit::UILabel(&v);
            lbl->setText(name);
            lbl->setColor(0xBFDBFE);
            lbl->setFrame(40, y, 100, 24);
            m_labels.push_back(std::unique_ptr<uikit::UILabel>(lbl));
            auto *p = new uikit::UIProgressView(&v);
            p->setRange(0, 100);
            p->setValue(val);
            p->setTrackColor(0xFFFFFF55);
            p->setProgressColor(color);
            p->setFrame(150, y + 4, w - 190, 12);
            m_bars.push_back(std::unique_ptr<uikit::UIProgressView>(p));
        };
        bar("Humidity", 68, 0x38BDF8, 380);
        bar("Wind", 45, 0x34D399, 420);
        bar("UV index", 80, 0xFBBF24, 460);
    }

    void onDestroy() override {
        launcher::vc_log("Weather", "onDestroy");
        m_bars.clear();
        m_labels.clear();
        m_desc.reset();
        m_temp.reset();
        m_icon.reset();
        m_back.reset();
    }

private:
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UILabel> m_icon;
    std::unique_ptr<uikit::UILabel> m_temp;
    std::unique_ptr<uikit::UILabel> m_desc;
    std::vector<std::unique_ptr<uikit::UILabel>> m_labels;
    std::vector<std::unique_ptr<uikit::UIProgressView>> m_bars;
};

uikit::UIViewController *weather_app_create(uikit::UINavigationController *nav)
{
    auto *app = new WeatherApp();
    app->setNav(nav);
    return app;
}
