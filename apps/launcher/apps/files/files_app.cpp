/**
 * @file files_app.cpp — Files: list with size rows; tap to highlight.
 */

#include "../../app_common.h"
#include "../../app_factory.h"

class FilesApp : public launcher::Activity {
public:
    void onCreate() override {
        launcher::vc_log("Files", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Files");
        m_back  = launcher::make_back(this, v, m_nav);

        static const struct { const char *name; const char *size; } kFiles[] = {
            { "report_q3.pdf",  "2.4 MB" },
            { "photo_0712.jpg", "1.1 MB" },
            { "notes.txt",      "12 KB"  },
            { "app.bin",        "480 KB" },
            { "backup.zip",     "86 MB"  },
        };
        for (int i = 0; i < 5; i++) {
            auto row = std::make_unique<uikit::UIButton>(&v);
            row->setBackgroundColor(0x1E293B);
            row->setCornerRadius(12);
            row->setFrame(20, 110 + i * 66, w - 40, 56);
            char title[64];
            std::snprintf(title, sizeof(title), "%s   %s",
                          kFiles[i].name, kFiles[i].size);
            row->setTitle(title);
            row->setTitleColor(0xE2E8F0);
            lv_obj_t *lbl = lv_obj_get_child(::UIView_native(row->native()), 0);
            if (lbl) {
                lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
                lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, 0);
            }
            row->onClick([this, i](uikit::UIButton *) {
                std::printf("[Files] selected: %s\n", kFiles[i].name);
            });
            m_rows.push_back(std::move(row));
        }
    }

    void onDestroy() override {
        launcher::vc_log("Files", "onDestroy");
        m_rows.clear();
        m_back.reset();
        m_title.reset();
    }

private:
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::vector<std::unique_ptr<uikit::UIButton>> m_rows;
};

uikit::UIViewController *files_app_create(uikit::UINavigationController *nav)
{
    auto *app = new FilesApp();
    app->setNav(nav);
    return app;
}
