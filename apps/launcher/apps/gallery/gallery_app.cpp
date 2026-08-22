/**
 * @file gallery_app.cpp — Gallery: color photo grid; tap a photo to preview.
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class GalleryApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Gallery", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Gallery");
        m_back  = launcher::make_back(this, v, m_nav);

        /* Preview strip */
        m_preview = std::make_unique<uikit::UIView>(&v);
        m_preview->setFrame(20, 90, w - 40, 140);
        m_preview->setBackgroundColor(0x1E293B);
        m_preview->setCornerRadius(16);

        static const uint32_t kColors[] = {
            0x38BDF8, 0x34D399, 0xF472B6, 0xFBBF24,
            0xA78BFA, 0xF87171, 0x2DD4BF, 0xFB923C,
            0x818CF8, 0x4ADE80, 0xE879F9, 0x60A5FA,
        };
        const int cols = 4, rows = 3;
        const int cell_w = (w - 60) / cols;
        const int cell_h = 90;
        for (int i = 0; i < 12; i++) {
            auto tile = std::make_unique<uikit::UIButton>(&v);
            tile->setBackgroundColor(kColors[i]);
            tile->setCornerRadius(12);
            tile->setFrame(20 + (i % cols) * cell_w, 250 + (i / cols) * (cell_h + 10),
                           cell_w - 10, cell_h);
            lv_obj_set_style_bg_opa(::UIView_native(tile->native()), LV_OPA_COVER, 0);
            tile->onClick([this, c = kColors[i]](uikit::UIButton *) {
                if (m_preview) {
                    m_preview->setBackgroundColor(c);
                }
            });
            m_tiles.push_back(std::move(tile));
        }
    }

    void onDestroy() override {
        launcher::vc_log("Gallery", "onDestroy");
        m_tiles.clear();
        m_preview.reset();
        m_back.reset();
        m_title.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::unique_ptr<uikit::UIView> m_preview;
    std::vector<std::unique_ptr<uikit::UIButton>> m_tiles;
};

uikit::UIViewController *gallery_app_create(uikit::UINavigationController *nav)
{
    auto *app = new GalleryApp();
    app->setNav(nav);
    return app;
}
