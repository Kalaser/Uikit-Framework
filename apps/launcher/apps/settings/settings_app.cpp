/**
 * @file settings_app.cpp — Settings: switches, sliders, text, theme, stack.
 *
 * Material-style grouped cards: each section (Connectivity / Display /
 * Storage / Appearance) lives in a rounded surface card using the shared
 * launcher theme tokens.
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
        v.setBackgroundColor(launcher_theme::kBg);
        const int w = lv_disp_get_hor_res(NULL);
        const int cw = w - 2 * launcher_theme::kSpaceMd;   /* card width  */

        m_title = launcher::make_title(v, "Settings");
        m_back  = launcher::make_back(this, v, m_nav);

        /* ── Card 1: Connectivity ───────────────────────────────────────── */
        auto *c1 = new uikit::UIView(&v);
        launcher_theme::style_card(*c1, launcher_theme::kRadiusCard, launcher_theme::kSurface);
        c1->setFrame(launcher_theme::kSpaceMd, 110, cw, 176);
        m_cards.push_back(std::unique_ptr<uikit::UIView>(c1));
        make_switch_row(*c1, "Wi-Fi", 0x38BDF8, 12);
        make_switch_row(*c1, "Bluetooth", 0x818CF8, 68);
        make_switch_row(*c1, "Do Not Disturb", 0xF472B6, 124);

        /* ── Card 2: Display (brightness slider → progress) ─────────────── */
        auto *c2 = new uikit::UIView(&v);
        launcher_theme::style_card(*c2, launcher_theme::kRadiusCard, launcher_theme::kSurface);
        c2->setFrame(launcher_theme::kSpaceMd, 298, cw, 118);
        m_cards.push_back(std::unique_ptr<uikit::UIView>(c2));
        make_slider_row(*c2, 0x22D3EE, 12);

        /* ── Card 3: Storage + device name ──────────────────────────────── */
        auto *c3 = new uikit::UIView(&v);
        launcher_theme::style_card(*c3, launcher_theme::kRadiusCard, launcher_theme::kSurface);
        c3->setFrame(launcher_theme::kSpaceMd, 428, cw, 130);
        m_cards.push_back(std::unique_ptr<uikit::UIView>(c3));

        auto *stor_label = new uikit::UILabel(c3);
        stor_label->setText("Storage  62%");
        stor_label->setColor(launcher_theme::kTextMid);
        stor_label->setFrame(launcher_theme::kSpaceLg, 16, cw - 40, 24);
        m_labels.push_back(std::unique_ptr<uikit::UILabel>(stor_label));

        auto *stor = new uikit::UIProgressView(c3);
        stor->setFrame(launcher_theme::kSpaceLg, 46, cw - 40, 10);
        stor->setTrackColor(launcher_theme::kBorder);
        stor->setProgressColor(0x34D399);
        stor->setValue(62);
        m_progresses.push_back(std::unique_ptr<uikit::UIProgressView>(stor));

        auto *dev = new uikit::UITextField(c3);
        dev->setFrame(launcher_theme::kSpaceLg, 74, cw - 40, 44);
        dev->setText("My Device");
        dev->setTextColor(launcher_theme::kTextHi);
        dev->setPlaceholder("Device name…");
        lv_obj_set_style_bg_color(::UIView_native(dev->native()),
                                  lv_color_hex(launcher_theme::kSurfaceAlt), 0);
        lv_obj_set_style_bg_opa(::UIView_native(dev->native()), LV_OPA_COVER, 0);
        lv_obj_set_style_radius(::UIView_native(dev->native()), launcher_theme::kRadiusCtl, 0);
        lv_obj_set_style_border_width(::UIView_native(dev->native()), 0, 0);
        m_fields.push_back(std::unique_ptr<uikit::UITextField>(dev));
        dev->onChange([](const char *t) { std::printf("[Settings] device name: %s\n", t ? t : ""); });

        /* ── Card 4: Appearance (theme switch + component showcase) ─────── */
        auto *c4 = new uikit::UIView(&v);
        launcher_theme::style_card(*c4, launcher_theme::kRadiusCard, launcher_theme::kSurface);
        c4->setFrame(launcher_theme::kSpaceMd, 570, cw, 152);
        m_cards.push_back(std::unique_ptr<uikit::UIView>(c4));

        m_theme = std::make_unique<uikit::UIButton>(c4);
        m_theme->setTitle("Switch theme (animate)");
        m_theme->setTitleColor(launcher_theme::kTextHi);
        m_theme->setBackgroundColor(launcher_theme::kAccent);
        /* 16 < h/2 (44): half-pill radius hangs LVGL 8.3 redraws. */
        m_theme->setCornerRadius(launcher_theme::kRadiusCard);
        launcher_theme::style_pressed(*m_theme, launcher_theme::kAccentAlt);
        m_theme->setFrame(launcher_theme::kSpaceLg, 16, cw - 40, 44);
        m_theme->onClick([this](uikit::UIButton *) {
            auto *t = ::UITheme_get_default();
            t->primary_color = (t->primary_color == launcher_theme::kAccent)
                                   ? launcher_theme::kAccentAlt
                                   : launcher_theme::kAccent;
            m_theme->setBackgroundColor(t->primary_color);
            m_theme->animate(::UIANIM_PROP_OPACITY, 255, 300);
        });

        /* UIStackView showcase: horizontal color chips */
        m_stack = std::make_unique<uikit::UIStackView>(c4);
        m_stack->setFrame(launcher_theme::kSpaceLg, 72, cw - 40, 56);
        m_stack->setAxis(::UIStackAxis_HORIZONTAL);
        m_stack->setSpacing(12);
        for (uint32_t c : { 0x38BDF8u, 0x34D399u, 0xF472B6u, 0xFBBF24u }) {
            auto chip = std::make_unique<uikit::UIView>();
            chip->setSize(70, 52);
            chip->setBackgroundColor(c);
            chip->setCornerRadius(14);
            chip->setOpacity(0);
            chip->animate(::UIANIM_PROP_OPACITY, 255, 500);
            m_stack->addArrangedSubview(*chip);
            m_chips.push_back(std::move(chip));
        }
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
        m_cards.clear();
        m_back.reset();
        m_title.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    std::unique_ptr<uikit::UILabel> m_title;
    std::unique_ptr<uikit::UIButton> m_back;

    std::vector<std::unique_ptr<uikit::UIView>>         m_cards;
    std::vector<std::unique_ptr<uikit::UILabel>>        m_switch_labels;
    std::vector<std::unique_ptr<uikit::UISwitch>>       m_switches;
    std::unique_ptr<uikit::UILabel>                     m_bri_label;
    std::unique_ptr<uikit::UISlider>                    m_bri_slider;
    std::unique_ptr<uikit::UIProgressView>              m_bri_progress;
    std::vector<std::unique_ptr<uikit::UIProgressView>> m_progresses;
    std::vector<std::unique_ptr<uikit::UILabel>>        m_labels;
    std::vector<std::unique_ptr<uikit::UITextField>>    m_fields;
    std::unique_ptr<uikit::UIButton>                    m_theme;
    std::unique_ptr<uikit::UIStackView>                 m_stack;
    std::vector<std::unique_ptr<uikit::UIView>>         m_chips;

    void make_switch_row(uikit::UIView &card, const char *name, uint32_t on_color, int32_t y)
    {
        const int w = lv_disp_get_hor_res(NULL);
        auto *label = new uikit::UILabel(&card);
        label->setText(name);
        label->setColor(launcher_theme::kTextHi);
        label->setFont(launcher_theme::kFontBody);
        label->setFrame(launcher_theme::kSpaceLg, y, 220, 40);
        m_switch_labels.push_back(std::unique_ptr<uikit::UILabel>(label));

        auto *sw = new uikit::UISwitch(&card);
        sw->setOnColor(on_color);
        sw->setFrame(w - 140, y + 4, 80, 32);
        sw->onChange([name](bool on) { std::printf("[Settings] %s: %s\n", name, on ? "ON" : "OFF"); });
        m_switches.push_back(std::unique_ptr<uikit::UISwitch>(sw));
    }

    void make_slider_row(uikit::UIView &card, uint32_t color, int32_t y)
    {
        const int w = lv_disp_get_hor_res(NULL);
        auto *label = new uikit::UILabel(&card);
        label->setText("Brightness");
        label->setColor(launcher_theme::kTextHi);
        label->setFont(launcher_theme::kFontBody);
        label->setFrame(launcher_theme::kSpaceLg, y, 220, 40);
        m_labels.push_back(std::unique_ptr<uikit::UILabel>(label));

        auto *slider = new uikit::UISlider(&card);
        slider->setRange(0, 100);
        slider->setValue(70);
        slider->setIndicatorColor(color);
        slider->setFrame(launcher_theme::kSpaceLg, y + 44, w - 64, 12);
        slider->onChange([this](int val) {
            std::printf("[Settings] brightness: %d\n", val);
            if (m_bri_progress) m_bri_progress->setValue(val);
        });
        m_bri_slider.reset(slider);

        auto *progress = new uikit::UIProgressView(&card);
        progress->setRange(0, 100);
        progress->setValue(70);
        progress->setTrackColor(launcher_theme::kBorder);
        progress->setProgressColor(color);
        progress->setFrame(launcher_theme::kSpaceLg, y + 70, w - 64, 8);
        m_bri_progress.reset(progress);
    }
};

uikit::UIViewController *settings_app_create(uikit::UINavigationController *nav)
{
    auto *app = new SettingsApp();
    app->setNav(nav);
    return app;
}
