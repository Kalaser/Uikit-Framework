/**
 * @file settings_xml_app.cpp — Settings page built from an XML layout.
 *
 * Declarative-UI demo: page structure lives in res/pages/settings.xml,
 * compiled to UIKit C code by tools/xml2c.py at build time. This controller
 * only builds the page and binds behavior to widgets by id.
 *
 * The current XML uses the light-theme schema (screen / app_bar /
 * section_header / setting_item) — a terminal-preferences style settings
 * page. Layout and styling are 100% XML; no layout code here.
 */

#include "../../app_common.h"
#include "../../app_factory.h"
#include "settings_xml.h"

#include <cstdio>
#include <memory>

class SettingsXmlApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("SettingsXml", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0xF7F7FA);

        /* 1. Build the page from XML (generated C code, no parser). */
        m_root = settings_xml_build(&m_ids);
        lv_obj_set_parent(::UIView_native(m_root), ::UIView_native(v.native()));
        lv_obj_clear_flag(::UIView_native(m_root), LV_OBJ_FLAG_SCROLLABLE);

        /* 2. Bind behavior by id — the only code in this controller. */
        if (m_ids.back) {
            ::UIView_add_callback((UIView *)m_ids.back, UIEVENT_CLICK,
                                  back_thunk, this);
        }
        if (m_ids.confirm_destructive_commands) {
            ::UIView_add_callback((UIView *)m_ids.confirm_destructive_commands,
                                  UIEVENT_VALUE_CHANGED,
                                  switch_thunk, (void *)"Confirm Destructive Commands");
        }
        if (m_ids.auto_reconnect) {
            ::UIView_add_callback((UIView *)m_ids.auto_reconnect,
                                  UIEVENT_VALUE_CHANGED,
                                  switch_thunk, (void *)"Auto Reconnect");
        }
        if (m_ids.keep_screen_awake) {
            ::UIView_add_callback((UIView *)m_ids.keep_screen_awake,
                                  UIEVENT_VALUE_CHANGED,
                                  switch_thunk, (void *)"Keep Screen Awake");
        }
    }

    void onDestroy() override {
        launcher::vc_log("SettingsXml", "onDestroy");
        memset(&m_ids, 0, sizeof(m_ids));   /* view tree is freed by the C layer */
        m_root = nullptr;
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    UIView *m_root = nullptr;
    SettingsXmlIds m_ids;

    static void back_thunk(UIEvent *ev, void *user_data)
    {
        (void)ev;
        auto *self = static_cast<SettingsXmlApp *>(user_data);
        if (self->m_nav) {
            self->m_nav->pop();
            launcher::schedule_delete(self);
        }
    }

    static void switch_thunk(UIEvent *ev, void *user_data)
    {
        std::printf("[SettingsXml] %s: %s\n",
                    static_cast<const char *>(user_data),
                    (ev && ev->value) ? "ON" : "OFF");
    }
};

uikit::UIViewController *settings_xml_app_create(uikit::UINavigationController *nav)
{
    auto *app = new SettingsXmlApp();
    app->setNav(nav);
    return app;
}
