/**
 * @file quicksettings.cpp — Quick settings panel (Android): system toggles
 *       pulled down from the status bar.
 */

#include "../app_common.h"
#include "../app_factory.h"

class QuickSettingsViewController : public launcher::Activity {
public:
    void onCreate() override {
        launcher::vc_log("QuickSettings", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0B1220);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Quick Settings");
        m_back  = launcher::make_back(this, v, m_nav);

        struct Toggle { const char *name; const char *icon; uint32_t color; };
        static const Toggle kToggles[] = {
            { "Wi-Fi",       LV_SYMBOL_WIFI,     0x38BDF8 },
            { "Bluetooth",   LV_SYMBOL_BLUETOOTH,0x818CF8 },
            { "Airplane",    LV_SYMBOL_UP,       0xFBBF24 },
            { "Flashlight",  LV_SYMBOL_EYE_OPEN, 0xF472B6 },
        };
        const int cell_w = (w - 40) / 2;
        for (int i = 0; i < 4; i++) {
            auto cell = std::make_unique<uikit::UIView>(&v);
            cell->setFrame(20 + (i % 2) * cell_w, 110 + (i / 2) * 130, cell_w - 12, 120);
            cell->setBackgroundColor(0x1E293B);
            cell->setCornerRadius(18);
            m_cells.push_back(std::move(cell));

            auto *icon = new uikit::UILabel(&v);
            icon->setText(kToggles[i].icon);
            icon->setColor(kToggles[i].color);
            icon->setAlignment(1);
            icon->setFont(&lv_font_montserrat_28);
            icon->setFrame(20 + (i % 2) * cell_w, 122 + (i / 2) * 130, cell_w - 12, 44);
            m_icons.push_back(std::unique_ptr<uikit::UILabel>(icon));

            auto *sw = new uikit::UISwitch(&v);
            sw->setOnColor(kToggles[i].color);
            sw->setFrame(20 + (i % 2) * cell_w + (cell_w - 12) / 2 - 40,
                         176 + (i / 2) * 130, 80, 32);
            sw->onChange([name = kToggles[i].name](bool on) {
                std::printf("[QuickSettings] %s: %s\n", name, on ? "ON" : "OFF");
            });
            m_switches.push_back(std::unique_ptr<uikit::UISwitch>(sw));
        }

        /* Brightness strip */
        auto *bright = new uikit::UILabel(&v);
        bright->setText("Brightness");
        bright->setColor(0xE2E8F0);
        bright->setFrame(24, 380, 140, 30);
        m_labels.push_back(std::unique_ptr<uikit::UILabel>(bright));

        m_slider = std::make_unique<uikit::UISlider>(&v);
        m_slider->setRange(0, 100);
        m_slider->setValue(70);
        m_slider->setIndicatorColor(0x22D3EE);
        m_slider->setFrame(24, 414, w - 48, 12);
        m_slider->onChange([this](int val) {
            std::printf("[QuickSettings] brightness: %d\n", val);
            if (m_progress) m_progress->setValue(val);
        });

        m_progress = std::make_unique<uikit::UIProgressView>(&v);
        m_progress->setRange(0, 100);
        m_progress->setValue(70);
        m_progress->setTrackColor(0x1E293B);
        m_progress->setProgressColor(0x22D3EE);
        m_progress->setFrame(24, 440, w - 48, 8);
    }

    void onDestroy() override {
        launcher::vc_log("QuickSettings", "onDestroy");
        m_progress.reset();
        m_slider.reset();
        m_labels.clear();
        m_switches.clear();
        m_icons.clear();
        m_cells.clear();
        m_back.reset();
        m_title.reset();
    }

private:
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;
    std::vector<std::unique_ptr<uikit::UIView>> m_cells;
    std::vector<std::unique_ptr<uikit::UILabel>> m_icons;
    std::vector<std::unique_ptr<uikit::UISwitch>> m_switches;
    std::vector<std::unique_ptr<uikit::UILabel>> m_labels;
    std::unique_ptr<uikit::UISlider> m_slider;
    std::unique_ptr<uikit::UIProgressView> m_progress;
};

uikit::UIViewController *launcher_quicksettings_create(uikit::UINavigationController *nav)
{
    auto *vc = new QuickSettingsViewController();
    vc->setNav(nav);
    return vc;
}
