/**
 * @file settings_app.cpp — Settings: switches, sliders, text, theme, stack.
 */

#include "../../app_common.h"
#include "../../app_factory.h"
#include <memory>
#include <vector>

class SettingsApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Settings", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0B1220);
        const int w = lv_disp_get_hor_res(NULL);

        m_title = launcher::make_title(v, "Settings");
        m_back  = launcher::make_back(this, v, m_nav);

        /* Rows: Wi-Fi / Bluetooth / DND */
        make_switch_row(v, "Wi-Fi", 0x38BDF8, 110);
        make_switch_row(v, "Bluetooth", 0x818CF8, 170);
        make_switch_row(v, "Do Not Disturb", 0xF472B6, 230);

        /* Brightness: slider → progress */
        make_slider_row(v, 0x22D3EE, 300);

        /* Storage */
        auto *stor = new uikit::UIProgressView(&v);
        stor->setFrame(20, 390, w - 40, 10);
        stor->setTrackColor(0x1E293B);
        stor->setProgressColor(0x34D399);
        stor->setValue(62);
        m_progresses.push_back(std::unique_ptr<uikit::UIProgressView>(stor));
        auto *stor_label = new uikit::UILabel(&v);
        stor_label->setText("Storage  62%");
        stor_label->setColor(0x94A3B8);
        stor_label->setFrame(20, 365, w - 40, 24);
        m_labels.push_back(std::unique_ptr<uikit::UILabel>(stor_label));

        /* Device name */
        auto *dev = new uikit::UITextField(&v);
        dev->setFrame(20, 430, w - 40, 44);
        dev->setText("My Device");
        dev->setTextColor(0xFFFFFF);
        dev->setPlaceholder("Device name…");
        m_fields.push_back(std::unique_ptr<uikit::UITextField>(dev));
        dev->onChange([](const char *t) { std::printf("[Settings] device name: %s\n", t ? t : ""); });

        /* Theme switch + animation */
        m_theme = std::make_unique<uikit::UIButton>(&v);
        m_theme->setTitle("Switch theme (animate)");
        m_theme->setTitleColor(0xFFFFFF);
        m_theme->setBackgroundColor(::UITheme_get_default()->primary_color);
        m_theme->setCornerRadius(22);
        m_theme->setFrame(20, 490, w - 40, 44);
        m_theme->onClick([this](uikit::UIButton *) {
            auto *t = ::UITheme_get_default();
            t->primary_color = (t->primary_color == 0x22D3EE) ? 0x34D399 : 0x22D3EE;
            m_theme->setBackgroundColor(t->primary_color);
            m_theme->animate(::UIANIM_PROP_OPACITY, 255, 300);
        });

        /* UIStackView showcase */
        m_stack = std::make_unique<uikit::UIStackView>(&v);
        m_stack->setFrame(20, 560, w - 40, 60);
        m_stack->setAxis(::UIStackAxis_HORIZONTAL);
        m_stack->setSpacing(12);
        for (uint32_t c : { 0x38BDF8u, 0x34D399u, 0xF472B6u, 0xFBBF24u }) {
            auto chip = std::make_unique<uikit::UIView>();
            chip->setSize(70, 56);
            chip->setBackgroundColor(c);
            chip->setCornerRadius(14);
            chip->setOpacity(0);
            chip->animate(::UIANIM_PROP_OPACITY, 255, 500);
            m_stack->addArrangedSubview(*chip);
            m_chips.push_back(std::move(chip));
        }

        auto *hint = new uikit::UILabel(&v);
        hint->setText("Watch the console for lifecycle logs");
        hint->setColor(0x64748B);
        hint->setFrame(20, 640, w - 40, 24);
        m_labels.push_back(std::unique_ptr<uikit::UILabel>(hint));
    }

    void onDestroy() override {
        launcher::vc_log("Settings", "onDestroy");
        m_chips.clear();
        m_stack.reset();
        m_theme.reset();
        m_fields.clear();
        m_progresses.clear();
        m_labels.clear();
        m_bri_progress.reset();
        m_bri_slider.reset();
        m_bri_label.reset();
        m_switches.clear();
        m_switch_labels.clear();
        m_back.reset();
        m_title.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;

    std::vector<std::unique_ptr<uikit::UILabel>>  m_switch_labels;
    std::vector<std::unique_ptr<uikit::UISwitch>> m_switches;
    std::unique_ptr<uikit::UILabel>               m_bri_label;
    std::unique_ptr<uikit::UISlider>              m_bri_slider;
    std::unique_ptr<uikit::UIProgressView>        m_bri_progress;
    std::vector<std::unique_ptr<uikit::UIProgressView>> m_progresses;
    std::vector<std::unique_ptr<uikit::UILabel>>        m_labels;
    std::vector<std::unique_ptr<uikit::UITextField>>    m_fields;
    std::unique_ptr<uikit::UIButton>                    m_theme;
    std::unique_ptr<uikit::UIStackView>                 m_stack;
    std::vector<std::unique_ptr<uikit::UIView>>         m_chips;

    void make_switch_row(uikit::UIView &v, const char *name, uint32_t on_color, int32_t y)
    {
        const int w = lv_disp_get_hor_res(NULL);
        auto *label = new uikit::UILabel(&v);
        label->setText(name);
        label->setColor(0xE2E8F0);
        label->setFont(&lv_font_montserrat_16);
        label->setFrame(24, y, 200, 40);
        m_switch_labels.push_back(std::unique_ptr<uikit::UILabel>(label));

        auto *sw = new uikit::UISwitch(&v);
        sw->setOnColor(on_color);
        sw->setFrame(w - 120, y + 4, 80, 32);
        sw->onChange([name](bool on) { std::printf("[Settings] %s: %s\n", name, on ? "ON" : "OFF"); });
        m_switches.push_back(std::unique_ptr<uikit::UISwitch>(sw));
    }

    void make_slider_row(uikit::UIView &v, uint32_t color, int32_t y)
    {
        const int w = lv_disp_get_hor_res(NULL);
        auto *label = new uikit::UILabel(&v);
        label->setText("Brightness");
        label->setColor(0xE2E8F0);
        label->setFont(&lv_font_montserrat_16);
        label->setFrame(24, y, 200, 40);
        m_labels.push_back(std::unique_ptr<uikit::UILabel>(label));

        auto *slider = new uikit::UISlider(&v);
        slider->setRange(0, 100);
        slider->setValue(70);
        slider->setIndicatorColor(color);
        slider->setFrame(24, y + 44, w - 48, 12);
        slider->onChange([this](int val) {
            std::printf("[Settings] brightness: %d\n", val);
            if (m_bri_progress) m_bri_progress->setValue(val);
        });
        m_bri_slider.reset(slider);

        auto *progress = new uikit::UIProgressView(&v);
        progress->setRange(0, 100);
        progress->setValue(70);
        progress->setTrackColor(0x1E293B);
        progress->setProgressColor(color);
        progress->setFrame(24, y + 70, w - 48, 8);
        m_bri_progress.reset(progress);
    }
};

uikit::UIViewController *settings_app_create(uikit::UINavigationController *nav)
{
    auto *app = new SettingsApp();
    app->setNav(nav);
    return app;
}
