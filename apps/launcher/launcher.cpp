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

#include <algorithm>
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

/* Long-press context passed through the C event callback (Android-style
 * context menu trigger). Owned by the launcher (vector of unique_ptr).
 * The thunk is defined after LauncherViewController (needs a complete type). */
struct TileContext {
    class LauncherViewController *launcher;
    const AppDescriptor *app;
};

/** One app icon on the home screen: a colored round icon container (tappable,
 *  long-pressable) plus its caption label below it. */
struct AppTile {
    std::unique_ptr<uikit::UIButton> icon;
    std::unique_ptr<uikit::UILabel>  name;
};

/* Defined after LauncherViewController (needs a complete type). */
static void tile_long_press_thunk(UIEvent *ev, void *user_data);

/* ══ Launcher home screen ─────────────────────────────────────────────────── */

class LauncherViewController : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    /* ── App launch & system entries (Android/iOS style) ────────────────── */

    /** Launch an app and record it in the recent-apps list. */
    void launch_app(const AppDescriptor &app)
    {
        if (!m_nav) return;
        m_nav->push(app.create(m_nav));
        auto it = std::find(m_recents.begin(), m_recents.end(), &app);
        if (it != m_recents.end()) {
            m_recents.erase(it);
        }
        m_recents.insert(m_recents.begin(), &app);
        if (m_recents.size() > 6) {
            m_recents.pop_back();
        }
        std::printf("[Launcher] launch: %s\n", app.id);
    }

    void open_app_menu(const AppDescriptor &app)
    {
        if (m_nav) m_nav->push(launcher_app_menu_create(m_nav, app));
    }

    void open_recents()
    {
        if (m_nav) m_nav->push(launcher_recents_create(m_nav, m_recents));
    }

    void open_drawer()
    {
        if (m_nav) m_nav->push(launcher_drawer_create(m_nav));
    }

    void open_quicksettings()
    {
        if (m_nav) m_nav->push(launcher_quicksettings_create(m_nav));
    }

    /** Most-recent-first list (exposed for the recents page & self-check). */
    const std::vector<const AppDescriptor *> &recents() const { return m_recents; }

    void onCreate() override {
        launcher::vc_log("Launcher", "onCreate");
        auto v = getView();
        v.setBackgroundColor(launcher_theme::kWallpaper);

        build_status_bar(v);
        build_search(v);
        build_desktop(v);
        build_page_indicator(v);
        build_dock(v);

        /* Wall-paper fallback in case a parent style overrides our local style. */
        lv_obj_set_style_bg_color(::UIView_native(v.native()), lv_color_hex(launcher_theme::kWallpaper),
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(::UIView_native(v.native()), LV_OPA_COVER,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(launcher_theme::kWallpaper), 0);
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
        m_tile_ctxs.clear();
        m_dock_buttons.clear();
        m_app_names.clear();
        m_app_buttons.clear();
        m_pages.reset();
        m_scroll.reset();
        m_drawer_btn.reset();
        m_search.reset();
        m_recents_btn.reset();
        m_battery.reset();
        m_qs_btn.reset();
        m_time.reset();
        m_dock.reset();
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    lv_timer_t *m_clock_timer = nullptr;

    std::unique_ptr<uikit::UILabel> m_time;
    std::unique_ptr<uikit::UIButton> m_qs_btn;      /* status bar → quick settings */
    std::unique_ptr<uikit::UIButton> m_recents_btn; /* status bar → recents */
    std::unique_ptr<uikit::UILabel> m_battery;
    std::unique_ptr<uikit::UITextField> m_search;
    std::unique_ptr<uikit::UIButton> m_drawer_btn;  /* search row → app drawer */
    std::unique_ptr<uikit::UIScrollView> m_scroll;
    std::unique_ptr<uikit::UIPageControl> m_pages;
    std::vector<std::unique_ptr<uikit::UIButton>> m_app_buttons;
    std::vector<std::unique_ptr<uikit::UILabel>>  m_app_names;
    std::vector<std::unique_ptr<uikit::UIButton>> m_dock_buttons;
    std::unique_ptr<uikit::UIView> m_dock;
    std::vector<const AppDescriptor *> m_recents;   /* most-recent first */
    std::vector<std::unique_ptr<TileContext>> m_tile_ctxs;

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
        m_time->setColor(launcher_theme::kTextHi);
        m_time->setFont(&lv_font_montserrat_16);

        /* Wi-Fi glyph opens Quick Settings (Android pull-down semantics) */
        m_qs_btn = std::make_unique<uikit::UIButton>(&v);
        m_qs_btn->setTitle(LV_SYMBOL_WIFI);
        m_qs_btn->setTitleColor(launcher_theme::kTextHi);
        lv_obj_set_style_bg_opa(::UIView_native(m_qs_btn->native()), LV_OPA_TRANSP, 0);
        m_qs_btn->onClick([this](uikit::UIButton *) { open_quicksettings(); });

        /* Recents (Android Recents / iOS app switcher) */
        m_recents_btn = std::make_unique<uikit::UIButton>(&v);
        m_recents_btn->setTitle(LV_SYMBOL_LOOP);
        m_recents_btn->setTitleColor(launcher_theme::kTextHi);
        lv_obj_set_style_bg_opa(::UIView_native(m_recents_btn->native()), LV_OPA_TRANSP, 0);
        m_recents_btn->onClick([this](uikit::UIButton *) { open_recents(); });

        m_battery = std::make_unique<uikit::UILabel>(&v);
        m_battery->setText(LV_SYMBOL_BATTERY_FULL);
        m_battery->setColor(launcher_theme::kSuccess);
    }

    void build_search(uikit::UIView &v)
    {
        m_search = std::make_unique<uikit::UITextField>(&v);
        m_search->setPlaceholder("Search apps…");
        m_search->setTextColor(launcher_theme::kTextHi);
        lv_obj_set_style_bg_color(::UIView_native(m_search->native()), lv_color_hex(launcher_theme::kSurface), 0);
        lv_obj_set_style_bg_opa(::UIView_native(m_search->native()), LV_OPA_COVER, 0);
        lv_obj_set_style_radius(::UIView_native(m_search->native()), 22, 0);
        lv_obj_set_style_border_width(::UIView_native(m_search->native()), 0, 0);
        m_search->onChange([](const char *t) {
            std::printf("[Launcher] search: %s\n", t ? t : "");
        });

        /* "All apps" button opens the app drawer */
        m_drawer_btn = std::make_unique<uikit::UIButton>(&v);
        m_drawer_btn->setTitle(LV_SYMBOL_LIST " All");
        m_drawer_btn->setTitleColor(launcher_theme::kTextMid);
        m_drawer_btn->setBackgroundColor(launcher_theme::kSurface);
        m_drawer_btn->setCornerRadius(22);
        m_drawer_btn->onClick([this](uikit::UIButton *) { open_drawer(); });
        set_button_font(*m_drawer_btn, &lv_font_montserrat_14);
    }

    void build_desktop(uikit::UIView &v)
    {
        m_scroll = std::make_unique<uikit::UIScrollView>(&v);
        m_scroll->setVerticalScroll(false);
        m_scroll->setHorizontalScroll(true);
        m_scroll->setScrollbarVisible(false);
        m_scroll->setBackgroundColor(launcher_theme::kWallpaper);
        m_scroll->setCornerRadius(0);
        lv_obj_set_style_border_width(::UIView_native(m_scroll->native()), 0, 0);
        lv_obj_set_scroll_snap_x(::UIView_native(m_scroll->native()), LV_SCROLL_SNAP_CENTER);

        add_app_page(*m_scroll, kLauncherPage1, 6);
        add_app_page(*m_scroll, kLauncherPage2, 8);
    }

    void add_app_page(uikit::UIScrollView &scroll, const AppDescriptor *const *apps,
                      int count)
    {
        for (int i = 0; i < count; i++) {
            AppTile tile = make_app_tile(scroll, apps[i]);
            m_app_names.push_back(std::move(tile.name));
            m_app_buttons.push_back(std::move(tile.icon));
        }
    }

    /** Build a colored rounded icon with a glyph inside (tap-to-launch,
     *  long-press-to-menu) plus its caption label — Android-style tile. */
    AppTile make_app_tile(uikit::UIView &parent, const AppDescriptor *app)
    {
        AppTile tile;

        /* Icon container: colored rounded square, big glyph, soft shadow */
        tile.icon = std::make_unique<uikit::UIButton>(&parent);
        tile.icon->setTitle(app->icon);
        tile.icon->setTitleColor(launcher_theme::kTextHi);
        tile.icon->setBackgroundColor(app->color);
        tile.icon->setCornerRadius(launcher_theme::kRadiusIcon);
        set_button_font(*tile.icon, &lv_font_montserrat_48);
        lv_obj_t *ico = ::UIView_native(tile.icon->native());
        lv_obj_set_style_shadow_width(ico, 18, 0);
        lv_obj_set_style_shadow_color(ico, lv_color_hex(0x000000), 0);
        lv_obj_set_style_shadow_opa(ico, 80, 0);
        tile.icon->onClick([this, app](uikit::UIButton *) { launch_app(*app); });

        /* Long press → app context menu (C event callback keeps ctx alive) */
        auto ctx = std::make_unique<TileContext>(TileContext{ this, app });
        ::UIView_add_callback(tile.icon->native(), UIEVENT_LONG_PRESS,
                              tile_long_press_thunk, ctx.get());
        m_tile_ctxs.push_back(std::move(ctx));

        /* Caption under the icon */
        tile.name = std::make_unique<uikit::UILabel>(&parent);
        tile.name->setText(app->name);
        launcher_theme::style_caption(*tile.name);
        tile.name->setAlignment(1);
        return tile;
    }

    void build_page_indicator(uikit::UIView &v)
    {
        m_pages = std::make_unique<uikit::UIPageControl>(&v);
        m_pages->setPageCount(2);
        m_pages->setCurrentPage(0);
        m_pages->setCurrentColor(launcher_theme::kTextHi);
        m_pages->setPageColor(launcher_theme::kBorder);
    }

    void build_dock(uikit::UIView &v)
    {
        /* Dock bar background — uikit::UIView(&v) creates a new owned lv_obj
         * parented to the launcher view (never wrap an existing view's
         * pointer here — that edits the wrapped object itself). */
        auto *dock = new uikit::UIView(&v);
        dock->setBackgroundColor(launcher_theme::kSurface);
        lv_obj_set_style_bg_opa(::UIView_native(dock->native()), 200, 0);
        m_dock = std::unique_ptr<uikit::UIView>(dock);

        for (int i = 0; i < 4; i++) {
            const AppDescriptor *app = kLauncherDock[i];
            auto btn = std::make_unique<uikit::UIButton>(&v);
            char title[64];
            std::snprintf(title, sizeof(title), "%s\n%s", app->icon, app->name);
            btn->setTitle(title);
            btn->setTitleColor(launcher_theme::kTextMid);
            lv_obj_set_style_bg_opa(::UIView_native(btn->native()), LV_OPA_TRANSP, 0);
            set_button_font(*btn, launcher_theme::kFontCap);
            /* Tactile pressed feedback on the dock tile */
            lv_obj_set_style_bg_color(::UIView_native(btn->native()),
                                      lv_color_hex(launcher_theme::kSurfaceAlt),
                                      LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_opa(::UIView_native(btn->native()), 200,
                                    LV_PART_MAIN | LV_STATE_PRESSED);
            btn->onClick([this, app](uikit::UIButton *) { launch_app(*app); });

            auto ctx = std::make_unique<TileContext>(TileContext{ this, app });
            ::UIView_add_callback(btn->native(), UIEVENT_LONG_PRESS,
                                  tile_long_press_thunk, ctx.get());
            m_tile_ctxs.push_back(std::move(ctx));
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

        /* Status bar (pinned top): time | qs · recents · battery */
        m_time->setFrame(16, 10, 80, 26);
        m_qs_btn->setFrame(w - 136, 6, 36, 32);
        m_recents_btn->setFrame(w - 96, 6, 36, 32);
        m_battery->setFrame(w - 48, 10, 40, 26);

        /* Search bar + drawer button */
        m_search->setFrame(20, 56, w - 120, 44);
        m_drawer_btn->setFrame(w - 92, 56, 72, 44);

        /* Desktop scroll area (between search and dock) */
        m_scroll->setFrame(0, 112, w, h - 112 - dock_h - 24);

        /* App icons: 88×88 rounded squares centered in each column,
         * caption below. 6 per page, extra apps flow onto page 2 row 3. */
        const int colw = w / 3;
        const int icon_sz = 88;
        for (int idx = 0; idx < (int)m_app_buttons.size(); idx++) {
            int page = idx / 6, i = idx % 6;
            int col = i % 3, row = i / 3;
            int cx = page * w + col * colw + colw / 2;
            int y = 30 + row * 184;
            m_app_buttons[idx]->setFrame(cx - icon_sz / 2, y, icon_sz, icon_sz);
            if (idx < (int)m_app_names.size()) {
                m_app_names[idx]->setFrame(cx - 70, y + icon_sz + 6, 140, 22);
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

/** Long-press on an app tile opens its context menu (defined after the
 *  launcher class so the member call compiles). */
static void tile_long_press_thunk(UIEvent *ev, void *user_data)
{
    (void)ev;
    auto *ctx = static_cast<TileContext *>(user_data);
    if (ctx && ctx->launcher) {
        ctx->launcher->open_app_menu(*ctx->app);
    }
}

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
    theme->primary_color = launcher_theme::kAccent;
    theme->background_color = launcher_theme::kWallpaper;

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
        const AppDescriptor *const *all[] = { kLauncherPage1, kLauncherPage2, kLauncherDock };
        const int counts[] = { 6, 8, 4 };
        for (int g = 0; g < 3; g++) {
            for (int i = 0; i < counts[g]; i++) {
                const AppDescriptor &meta = *all[g][i];
                std::printf("== app: %s ==\n", meta.name);
                launcher->launch_app(meta);   /* records into recents too */
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
        /* System UI pages (drawer / recents / quick settings / menu) */
        auto run_page = [&](const char *name, uikit::UIViewController *vc) {
            std::printf("== system: %s ==\n", name);
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
        };
        run_page("drawer", launcher_drawer_create(&nav));
        run_page("recents", launcher_recents_create(&nav, launcher->recents()));
        run_page("quicksettings", launcher_quicksettings_create(&nav));
        run_page("app_menu", launcher_app_menu_create(&nav, kLauncherApps[0]));
        std::printf("app self-check: all apps OK\n");

        lv_mem_monitor_t mon;
        lv_mem_monitor(&mon);
        std::printf("[mem] LVGL pool: total=%uB used=%u%% free=%uB (max_used=%uB) blocks=%u frag=%u%%\n",
                    (unsigned)mon.total_size, (unsigned)mon.used_pct,
                    (unsigned)mon.free_size, (unsigned)mon.max_used,
                    (unsigned)mon.used_cnt, (unsigned)mon.frag_pct);

        WinDriver_deinit();
        return 0;
    }

    WinDriver_run();

    /* Resource diagnostic: how much of the LVGL pool did the launcher use? */
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    std::printf("[mem] LVGL pool: total=%uB used=%u%% free=%uB (max_used=%uB) blocks=%u frag=%u%%\n",
                (unsigned)mon.total_size, (unsigned)mon.used_pct,
                (unsigned)mon.free_size, (unsigned)mon.max_used,
                (unsigned)mon.used_cnt, (unsigned)mon.frag_pct);

    /* nav destructor tears down the launcher (onDestroy → delete) */
    return 0;
}
