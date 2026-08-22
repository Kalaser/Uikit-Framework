/**
 * @file maps_app.cpp — Maps: grid backdrop + location marker + zoom.
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class MapsApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Maps", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0B1220);
        const int w = lv_disp_get_hor_res(NULL);
        const int h = lv_disp_get_ver_res(NULL);

        m_back = launcher::make_back(this, v, m_nav);

        /* Map surface */
        m_map = std::make_unique<uikit::UIView>(&v);
        m_map->setFrame(0, 110, w, h - 220);
        m_map->setBackgroundColor(0x111C2E);

        /* Grid lines */
        for (int x = w / 6; x < w; x += w / 6) {
            auto line = std::make_unique<uikit::UIView>(&v);
            line->setFrame(x, 110, 1, h - 220);
            line->setBackgroundColor(0x22314A);
            m_grid.push_back(std::move(line));
        }
        for (int y = 110 + (h - 220) / 8; y < h - 110; y += (h - 220) / 8) {
            auto line = std::make_unique<uikit::UIView>(&v);
            line->setFrame(0, y, w, 1);
            line->setBackgroundColor(0x22314A);
            m_grid.push_back(std::move(line));
        }

        /* Location marker */
        m_marker = std::make_unique<uikit::UIView>(&v);
        m_marker->setFrame(w / 2 - 10, 110 + (h - 220) / 2 - 10, 20, 20);
        m_marker->setBackgroundColor(0xEF4444);
        m_marker->setCornerRadius(10);
        lv_obj_set_style_border_width(::UIView_native(m_marker->native()), 4, 0);
        lv_obj_set_style_border_color(::UIView_native(m_marker->native()),
                                      lv_color_hex(0xFFFFFF), 0);

        m_addr = std::make_unique<uikit::UILabel>(&v);
        m_addr->setText("Science Park Rd");
        m_addr->setColor(0xFFFFFF);
        m_addr->setAlignment(1);
        m_addr->setBackgroundColor(0x1E293B);
        m_addr->setFont(&lv_font_montserrat_16);
        m_addr->setFrame(w / 2 - 110, 110 + (h - 220) / 2 + 18, 220, 36);
        lv_obj_set_style_radius(::UIView_native(m_addr->native()), 8, 0);

        /* Zoom in/out */
        m_zin = std::make_unique<uikit::UIButton>(&v);
        m_zin->setTitle(LV_SYMBOL_PLUS);
        m_zin->setTitleColor(0xFFFFFF);
        m_zin->setBackgroundColor(0x1E293B);
        m_zin->setCornerRadius(20);
        m_zin->setFrame(w - 70, h - 180, 44, 44);
        m_zin->onClick([this](uikit::UIButton *) {
            m_zoom = (m_zoom < 3) ? m_zoom + 1 : 3;
            std::printf("[Maps] zoom: %d\n", m_zoom);
        });

        m_zout = std::make_unique<uikit::UIButton>(&v);
        m_zout->setTitle(LV_SYMBOL_MINUS);
        m_zout->setTitleColor(0xFFFFFF);
        m_zout->setBackgroundColor(0x1E293B);
        m_zout->setCornerRadius(20);
        m_zout->setFrame(w - 70, h - 130, 44, 44);
        m_zout->onClick([this](uikit::UIButton *) {
            m_zoom = (m_zoom > 0) ? m_zoom - 1 : 0;
            std::printf("[Maps] zoom: %d\n", m_zoom);
        });
    }

    void onDestroy() override {
        launcher::vc_log("Maps", "onDestroy");
        m_zout.reset();
        m_zin.reset();
        m_addr.reset();
        m_marker.reset();
        m_grid.clear();
        m_map.reset();
        m_back.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UIView> m_map;
    std::vector<std::unique_ptr<uikit::UIView>> m_grid;
    std::unique_ptr<uikit::UIView> m_marker;
    std::unique_ptr<uikit::UILabel> m_addr;
    std::unique_ptr<uikit::UIButton> m_zin;
    std::unique_ptr<uikit::UIButton> m_zout;
    int m_zoom = 1;
};

uikit::UIViewController *maps_app_create(uikit::UINavigationController *nav)
{
    auto *app = new MapsApp();
    app->setNav(nav);
    return app;
}
