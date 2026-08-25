/**
 * @file terminal_app.cpp — Terminal page (user XML schema).
 *
 * Page structure: res/pages/terminal.xml (terminal-preview card, quick
 * action cards, recent sessions, bottom tab bar). This controller only
 * builds the page and wires up the interactive ids.
 */

#include "../../app_common.h"
#include "../../app_factory.h"
#include "terminal_xml.h"

#include <cstdio>
#include <memory>

class TerminalApp : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void onCreate() override {
        launcher::vc_log("Terminal", "onCreate");
        auto v = getView();
        v.setBackgroundColor(0xF7F7FA);

        UIView *root = terminal_xml_build(&m_ids);
        lv_obj_set_parent(::UIView_native(root), ::UIView_native(v.native()));
        lv_obj_clear_flag(::UIView_native(root), LV_OBJ_FLAG_SCROLLABLE);

        /* Back */
        if (m_ids.back) {
            ::UIView_add_callback((UIView *)m_ids.back, UIEVENT_CLICK,
                                  back_thunk, this);
        }
        /* Quick actions */
        for (UIView *card : { m_ids.actConnect, m_ids.actScan, m_ids.actReboot }) {
            if (card) {
                ::UIView_add_callback(card, UIEVENT_CLICK, action_thunk, this);
            }
        }
        /* Recent device cards */
        for (UIView *card : { m_ids.factory_glass__24, m_ids.debug_board__07 }) {
            if (card) {
                ::UIView_add_callback(card, UIEVENT_CLICK, device_thunk, this);
            }
        }
        /* Bottom tabs */
        for (UIButton *tab : { m_ids.tab_terminal, m_ids.tab_devices, m_ids.tab_settings }) {
            if (tab) {
                ::UIView_add_callback((UIView *)tab, UIEVENT_CLICK, tab_thunk, this);
            }
        }
    }

    void onDestroy() override {
        launcher::vc_log("Terminal", "onDestroy");
        memset(&m_ids, 0, sizeof(m_ids));
    }

private:
    uikit::UINavigationController *m_nav = nullptr;
    TerminalXmlIds m_ids;

    static void back_thunk(UIEvent *ev, void *user_data)
    {
        (void)ev;
        auto *self = static_cast<TerminalApp *>(user_data);
        if (self->m_nav) {
            self->m_nav->pop();
            launcher::schedule_delete(self);
        }
    }

    static void action_thunk(UIEvent *ev, void *user_data)
    {
        (void)user_data;
        if (!ev) return;
        const char *name = "?";
        if (ev->target == (UIView *)static_cast<TerminalApp *>(user_data)->m_ids.actConnect) name = "Connect";
        else if (ev->target == (UIView *)static_cast<TerminalApp *>(user_data)->m_ids.actScan) name = "Scan";
        else if (ev->target == (UIView *)static_cast<TerminalApp *>(user_data)->m_ids.actReboot) name = "Reboot";
        std::printf("[Terminal] action: %s\n", name);
    }

    static void device_thunk(UIEvent *ev, void *user_data)
    {
        (void)user_data;
        std::printf("[Terminal] device tapped (x=%d y=%d)\n", ev->x, ev->y);
    }

    static void tab_thunk(UIEvent *ev, void *user_data)
    {
        (void)user_data;
        const char *name = "?";
        auto *self = static_cast<TerminalApp *>(user_data);
        if (ev->target == (UIView *)self->m_ids.tab_terminal) name = "Terminal";
        else if (ev->target == (UIView *)self->m_ids.tab_devices) name = "Devices";
        else if (ev->target == (UIView *)self->m_ids.tab_settings) name = "Settings";
        std::printf("[Terminal] tab: %s (TODO: switch page)\n", name);
    }
};

uikit::UIViewController *terminal_app_create(uikit::UINavigationController *nav)
{
    auto *app = new TerminalApp();
    app->setNav(nav);
    return app;
}
