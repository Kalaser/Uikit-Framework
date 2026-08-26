/**
 * @file camera_app.cpp — Camera: viewfinder + shutter + shot counter.
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class CameraApp : public launcher::Activity {
public:
    void onCreate() override {
        launcher::vc_log("Camera", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x000000);
        const int w = lv_disp_get_hor_res(NULL);
        const int h = lv_disp_get_ver_res(NULL);

        m_back = launcher::make_back(this, v, m_nav);

        /* Viewfinder area (darker surface) */
        m_viewfinder = std::make_unique<uikit::UIView>(&v);
        m_viewfinder->setFrame(0, 120, w, h - 240);
        m_viewfinder->setBackgroundColor(0x10141A);

        /* Rule-of-thirds grid lines */
        const int vf_y = 120, vf_h = h - 240;
        for (int x : { w / 3, 2 * w / 3 }) {
            auto line = std::make_unique<uikit::UIView>(&v);
            line->setFrame(x - 1, vf_y, 2, vf_h);
            line->setBackgroundColor(0xFFFFFF);
            lv_obj_set_style_bg_opa(::UIView_native(line->native()), 60, 0);
            m_lines.push_back(std::move(line));
        }
        for (int y : { vf_y + vf_h / 3, vf_y + 2 * vf_h / 3 }) {
            auto line = std::make_unique<uikit::UIView>(&v);
            line->setFrame(0, y, w, 2);
            line->setBackgroundColor(0xFFFFFF);
            lv_obj_set_style_bg_opa(::UIView_native(line->native()), 60, 0);
            m_lines.push_back(std::move(line));
        }

        /* Shutter button */
        m_shutter = std::make_unique<uikit::UIButton>(&v);
        m_shutter->setTitle(LV_SYMBOL_OK);
        m_shutter->setTitleColor(0xFFFFFF);
        m_shutter->setBackgroundColor(0xFFFFFF);
        m_shutter->setCornerRadius(30);
        m_shutter->setFrame(w / 2 - 30, h - 96, 60, 60);
        lv_obj_set_style_bg_color(::UIView_native(m_shutter->native()),
                                  lv_color_hex(0xFFFFFF), 0);
        m_shutter->onClick([this](uikit::UIButton *) {
            m_shots += 1;
            if (m_counter) {
                char buf[32];
                std::snprintf(buf, sizeof(buf), "Shots: %d", m_shots);
                m_counter->setText(buf);
            }
        });

        /* Shot counter */
        m_counter = std::make_unique<uikit::UILabel>(&v);
        m_counter->setText("Shots: 0");
        m_counter->setColor(0xFFFFFF);
        m_counter->setFont(&lv_font_montserrat_16);
        m_counter->setFrame(w / 2 - 60, h - 150, 120, 30);
    }

    void onDestroy() override {
        launcher::vc_log("Camera", "onDestroy");
        m_counter.reset();
        m_shutter.reset();
        m_lines.clear();
        m_viewfinder.reset();
        m_back.reset();
    }

private:
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UIView> m_viewfinder;
    std::vector<std::unique_ptr<uikit::UIView>> m_lines;
    std::unique_ptr<uikit::UIButton> m_shutter;
    std::unique_ptr<uikit::UILabel> m_counter;
    int m_shots = 0;
};

uikit::UIViewController *camera_app_create(uikit::UINavigationController *nav)
{
    auto *app = new CameraApp();
    app->setNav(nav);
    return app;
}
