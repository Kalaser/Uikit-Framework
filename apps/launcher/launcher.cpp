/**
 * @file launcher.cpp — Android-style launcher home screen.
 *
 * Each app is a standalone directory under apps/launcher/apps/ and is
 * registered through app_factory.h. The launcher only knows the AppMeta
 * tables — it never includes app internals.
 */

#include "UIKit.hpp"
#include "win_driver.h"
#include "app_common.h"
#include "app_factory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <memory>
#include <vector>

/* Enable Vista+ APIs (SetProcessDPIAware) before pulling in <windows.h> */
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#undef WINVER
#define WINVER 0x0600
#include <windows.h>

/* ── Small helpers ────────────────────────────────────────────────────────── */

/** Set the label font of a UIButton (its only child is the title label). */
static void set_button_font(uikit::UIButton &btn, const lv_font_t *font)
{
    lv_obj_t *lbl = lv_obj_get_child(::UIView_native(btn.native()), 0);
    if (lbl) {
        lv_obj_set_style_text_font(lbl, font, 0);
    }
}

/** Build a round-tile app icon button ("SYMBOL\nname") inside a container. */
static std::unique_ptr<uikit::UIButton> make_app_tile(
    uikit::UIView &parent, const AppMeta &app,
    std::function<void()> on_tap)
{
    auto btn = std::make_unique<uikit::UIButton>(&parent);
    char title[64];
    std::snprintf(title, sizeof(title), "%s\n%s", app.icon, app.name);
    btn->setTitle(title);
    btn->setTitleColor(0xFFFFFF);
    btn->setBackgroundColor(app.color);
    btn->setCornerRadius(30);
    set_button_font(*btn, &lv_font_montserrat_20);
    btn->onClick([on_tap](uikit::UIButton *) { on_tap(); });
    return btn;
}

/* ══ Launcher home screen ─────────────────────────────────────────────────── */

class LauncherViewController : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Launcher", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0x0F172A);

        build_status_bar(v);
        build_search(v);
        build_desktop(v);
        build_page_indicator(v);
        build_dock(v);

        /* Wall-paper fallback in case a parent style overrides our local style. */
        lv_obj_set_style_bg_color(::UIView_native(v.native()), lv_color_hex(0x0F172A),
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(::UIView_native(v.native()), LV_OPA_COVER,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x0F172A), 0);
        lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

        /* Initial responsive layout */
        relayout();

        /* Live clock */
        m_clock_timer = lv_timer_create([](lv_timer_t *t) {
            static_cast<LauncherViewController *>(t->user_data)->update_clock();
        }, 1000, this);
        update_clock();
    }

    void onStart() override  { launcher::vc_log("Launcher", "onStart"); }
    void onResume() override { launcher::vc_log("Launcher", "onResume"); }
    void onPause() override  { launcher::vc_log("Launcher", "onPause"); }
    void onStop() override   { launcher::vc_log("Launcher", "onStop"); }

    void onDestroy() override {
        launcher::vc_log("Launcher", "onDestroy");
        if (m_clock_timer) {
            lv_timer_del(m_clock_timer);
            m_clock_timer = nullptr;
        }
        m_dock_buttons.clear();
        m_app_buttons.clear();
        m_pages.reset();
        m_scroll.reset();
        m_search.reset();
        m_battery.reset();
        m_wifi.reset();
        m_time.reset();
        m_dock.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    lv_timer_t *m_clock_timer = nullptr;

    std::unique_ptr<uikit::UILabel> m_time;
    std::unique_ptr<uikit::UILabel> m_wifi;
    std::unique_ptr<uikit::UILabel> m_battery;
    std::unique_ptr<uikit::UITextField> m_search;
    std::unique_ptr<uikit::UIScrollView> m_scroll;
    std::unique_ptr<uikit::UIPageControl> m_pages;
    std::vector<std::unique_ptr<uikit::UIButton>> m_app_buttons;
    std::vector<std::unique_ptr<uikit::UIButton>> m_dock_buttons;
    std::unique_ptr<uikit::UIView> m_dock;

    void update_clock()
    {
        if (!m_time) return;
        std::time_t now = std::time(nullptr);
        std::tm *tm = std::localtime(&now);
        char buf[16];
        std::strftime(buf, sizeof(buf), "%H:%M", tm);
        m_time->setText(buf);
    }

    void build_status_bar(uikit::UIView &v)
    {
        m_time = std::make_unique<uikit::UILabel>(&v);
        m_time->setColor(0xFFFFFF);
        m_time->setFont(&lv_font_montserrat_16);

        m_wifi = std::make_unique<uikit::UILabel>(&v);
        m_wifi->setText(LV_SYMBOL_WIFI);
        m_wifi->setColor(0xFFFFFF);

        m_battery = std::make_unique<uikit::UILabel>(&v);
        m_battery->setText(LV_SYMBOL_BATTERY_FULL);
        m_battery->setColor(0x4ADE80);
    }

    void build_search(uikit::UIView &v)
    {
        m_search = std::make_unique<uikit::UITextField>(&v);
        m_search->setPlaceholder("Search apps…");
        m_search->setTextColor(0xFFFFFF);
        lv_obj_set_style_bg_color(::UIView_native(m_search->native()), lv_color_hex(0x1E293B), 0);
        lv_obj_set_style_bg_opa(::UIView_native(m_search->native()), LV_OPA_COVER, 0);
        lv_obj_set_style_radius(::UIView_native(m_search->native()), 22, 0);
        lv_obj_set_style_border_width(::UIView_native(m_search->native()), 0, 0);
        m_search->onChange([](const char *t) {
            std::printf("[Launcher] search: %s\n", t ? t : "");
        });
    }

    void build_desktop(uikit::UIView &v)
    {
        m_scroll = std::make_unique<uikit::UIScrollView>(&v);
        m_scroll->setVerticalScroll(false);
        m_scroll->setHorizontalScroll(true);
        m_scroll->setScrollbarVisible(false);
        m_scroll->setBackgroundColor(0x0F172A);
        m_scroll->setCornerRadius(0);
        lv_obj_set_style_border_width(::UIView_native(m_scroll->native()), 0, 0);
        lv_obj_set_scroll_snap_x(::UIView_native(m_scroll->native()), LV_SCROLL_SNAP_CENTER);

        add_app_page(*m_scroll, kLauncherPage1);
        add_app_page(*m_scroll, kLauncherPage2);
    }

    void add_app_page(uikit::UIScrollView &scroll, const AppMeta *apps)
    {
        for (int i = 0; i < 6; i++) {
            const AppMeta &app = apps[i];
            auto btn = make_app_tile(scroll, app, [this, &app]() {
                if (!m_nav) return;
                m_nav->push(app.create(m_nav));
            });
            m_app_buttons.push_back(std::move(btn));
        }
    }

    void build_page_indicator(uikit::UIView &v)
    {
        m_pages = std::make_unique<uikit::UIPageControl>(&v);
        m_pages->setPageCount(2);
        m_pages->setCurrentPage(0);
        m_pages->setCurrentColor(0xFFFFFF);
        m_pages->setPageColor(0x475569);
    }

    void build_dock(uikit::UIView &v)
    {
        /* Dock bar background — uikit::UIView(&v) creates a new owned lv_obj
         * parented to the launcher view (never wrap an existing view's
         * pointer here — that edits the wrapped object itself). */
        auto *dock = new uikit::UIView(&v);
        dock->setBackgroundColor(0x1E293B);
        lv_obj_set_style_bg_opa(::UIView_native(dock->native()), 200, 0);
        m_dock = std::unique_ptr<uikit::UIView>(dock);

        for (int i = 0; i < 4; i++) {
            const AppMeta &app = kLauncherDock[i];
            auto btn = std::make_unique<uikit::UIButton>(&v);
            char title[64];
            std::snprintf(title, sizeof(title), "%s\n%s", app.icon, app.name);
            btn->setTitle(title);
            btn->setTitleColor(0xE2E8F0);
            lv_obj_set_style_bg_opa(::UIView_native(btn->native()), LV_OPA_TRANSP, 0);
            set_button_font(*btn, &lv_font_montserrat_14);
            btn->onClick([this, &app](uikit::UIButton *) {
                if (!m_nav) return;
                m_nav->push(app.create(m_nav));
            });
            m_dock_buttons.push_back(std::move(btn));
        }
    }

    /* ── Responsive layout ──────────────────────────────────────────────── */

public:
    void relayout()
    {
        if (!m_time) {
            return;
        }
        const int w = lv_disp_get_hor_res(NULL);
        const int h = lv_disp_get_ver_res(NULL);
        const int dock_h = 88;

        /* Status bar (pinned top) */
        m_time->setFrame(16, 10, 80, 26);
        m_wifi->setFrame(w - 88, 10, 30, 26);
        m_battery->setFrame(w - 48, 10, 40, 26);

        /* Search bar */
        m_search->setFrame(20, 56, w - 40, 44);

        /* Desktop scroll area (between search and dock) */
        m_scroll->setFrame(0, 112, w, h - 112 - dock_h - 24);

        /* App tiles: 3 columns per page, centered in each column */
        int colw = w / 3;
        int tile_w = (colw > 160) ? 140 : (colw - 20);
        int idx = 0;
        for (int page = 0; page < 2; page++) {
            for (int i = 0; i < 6; i++, idx++) {
                if (idx >= (int)m_app_buttons.size()) break;
                int col = i % 3, row = i / 3;
                int x = page * w + col * colw + (colw - tile_w) / 2;
                int y = 24 + row * 190;
                m_app_buttons[idx]->setFrame(x, y, tile_w, 170);
            }
        }

        /* Page indicator (above the dock) */
        m_pages->setPosition((w - 16) / 2, h - dock_h - 22);

        /* Dock (pinned bottom) */
        m_dock->setFrame(0, h - dock_h, w, dock_h);
        int btn_w = (w - 40) / 4;
        for (int i = 0; i < 4 && i < (int)m_dock_buttons.size(); i++) {
            m_dock_buttons[i]->setFrame(20 + i * btn_w, h - dock_h + 12,
                                        btn_w - 10, dock_h - 24);
        }
    }
};

/* ══ Main ─────────────────────────────────────────────────────────────────── */

int main(int argc, char **argv)
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    /* Disable Windows DPI scaling so the coordinate space maps 1:1 to
     * physical pixels. */
    ::SetProcessDPIAware();

    int smoke_frames = 0;
    bool apps_check = false;
    if (argc > 1) {
        if (std::strcmp(argv[1], "--smoke") == 0) {
            smoke_frames = (argc > 2) ? std::atoi(argv[2]) : 180;
            std::printf("smoke mode: %d frames\n", smoke_frames);
        } else if (std::strcmp(argv[1], "--apps") == 0) {
            apps_check = true;
        }
    }

    lv_init();
    if (WinDriver_init(smoke_frames) != 0) {
        std::printf("WinDriver_init failed\n");
        return 1;
    }
    ::UIKit_init();

    /* Apply a theme */
    auto *theme = ::UITheme_get_default();
    theme->primary_color = 0x22D3EE;
    theme->background_color = 0x0F172A;

    auto *launcher = new LauncherViewController();
    uikit::UINavigationController nav(launcher);
    launcher->setNav(&nav);

    /* Responsive window: on resize, grow the nav surface + re-layout the
     * launcher home screen. */
    static uikit::UINavigationController *s_nav = &nav;
    static LauncherViewController *s_launcher = launcher;
    WinDriver_set_resize_callback([](int w, int h) {
        if (s_nav) {
            ::UINavigationController_resize(s_nav->c_ptr(), w, h);
        }
        if (s_launcher) {
            s_launcher->relayout();
        }
        std::printf("[launcher] resized to %dx%d\n", w, h);
    });

    /* CI self-check: launch every registered app, run a few frames, pop.
     * Exercises each app's onCreate/onDestroy + ownership without clicks. */
    if (apps_check) {
        std::printf("app self-check: launching all apps...\n");
        const AppMeta *all[] = { kLauncherPage1, kLauncherPage2, kLauncherDock };
        const int counts[] = { 6, 6, 4 };
        for (int g = 0; g < 3; g++) {
            for (int i = 0; i < counts[g]; i++) {
                const AppMeta &meta = all[g][i];
                std::printf("== app: %s ==\n", meta.name);
                auto *vc = meta.create(&nav);
                nav.push(vc);
                for (int f = 0; f < 6; f++) {
                    lv_tick_inc(16);
                    lv_timer_handler();
                }
                uikit::UIViewController *popped = nav.pop();
                if (popped) {
                    popped->destroy();
                    delete popped;
                }
            }
        }
        std::printf("app self-check: all apps OK\n");
        WinDriver_deinit();
        return 0;
    }

    WinDriver_run();

    /* nav destructor tears down the launcher (onDestroy → delete) */
    return 0;
}
