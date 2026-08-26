/**
 * @file app_common.h
 * @brief Shared helpers for the standalone launcher apps.
 */

#ifndef LAUNCHER_APP_COMMON_H
#define LAUNCHER_APP_COMMON_H

#include "UIKit.hpp"
#include "launcher_theme.h"
#include <cstdio>
#include <memory>

namespace launcher {

/** Android-style page base for launcher apps.
 *
 * Each launcher page is an Activity-shaped UIViewController. The navigation
 * controller acts as the task back-stack; startActivity() pushes a new page and
 * finish() pops the current page, returning ownership to the caller.
 */
class Activity : public uikit::UIViewController {
public:
    void setNav(uikit::UINavigationController *nav) { m_nav = nav; }

    void startActivity(uikit::UIViewController *controller)
    {
        if (m_nav && controller) {
            m_nav->push(controller);
        }
    }

    uikit::UIViewController *finish()
    {
        return m_nav ? m_nav->pop() : nullptr;
    }

protected:
    uikit::UINavigationController *navigationController() const { return m_nav; }

    uikit::UINavigationController *m_nav = nullptr;
};

/** Print a lifecycle log line. */
inline void vc_log(const char *who, const char *cb)
{
    std::printf("[%s] %s\n", who, cb);
}

/** Defer destroy()+delete() of a popped controller until the next LVGL tick
 *  so the click callback can return before the object is freed. */
inline void schedule_delete(uikit::UIViewController *vc)
{
    if (!vc) {
        return;
    }
    lv_timer_create([](lv_timer_t *t) {
        auto *ctrl = static_cast<uikit::UIViewController *>(t->user_data);
        lv_timer_del(t);
        ctrl->destroy();
        delete ctrl;
    }, 1, vc);
}

/** Build a "← Back" button that pops the controller and schedules its
 *  destruction. The returned button must be owned by the app controller
 *  (unique_ptr member) and released in onDestroy. */
inline std::unique_ptr<uikit::UIButton> make_back(launcher::Activity *self,
                                                  uikit::UIView &parent,
                                                  uikit::UINavigationController *nav)
{
    auto btn = std::make_unique<uikit::UIButton>(&parent);
    btn->setTitle(LV_SYMBOL_LEFT " Back");
    btn->setTitleColor(launcher_theme::kTextHi);
    btn->setBackgroundColor(launcher_theme::kSurface);
    /* Radius 16 < h/2 (44): a half-pill radius hangs LVGL 8.3 redraws. */
    btn->setCornerRadius(launcher_theme::kRadiusCard);
    launcher_theme::style_pressed(*btn, launcher_theme::kSurfaceAlt);
    btn->setFrame(launcher_theme::kSpaceLg, 56, 130, 44);
    lv_obj_t *lbl = lv_obj_get_child(::UIView_native(btn->native()), 0);
    if (lbl) {
        lv_obj_set_style_text_font(lbl, launcher_theme::kFontCap, 0);
    }
    btn->onClick([self, nav](uikit::UIButton *) {
        if (self && nav) {
            uikit::UIViewController *popped = self->finish();
            if (popped) {
                launcher::schedule_delete(popped);
            }
        }
    });
    return btn;
}

/** Page title centered under the status bar. */
inline std::unique_ptr<uikit::UILabel> make_title(uikit::UIView &parent, const char *text)
{
    auto lbl = std::make_unique<uikit::UILabel>(&parent);
    lbl->setText(text);
    lbl->setColor(launcher_theme::kTextHi);
    lbl->setAlignment(1);
    lbl->setFont(launcher_theme::kFontH1);
    const int w = lv_disp_get_hor_res(NULL);
    lbl->setFrame(0, 30, w, 40);
    return lbl;
}

} /* namespace launcher */

#endif /* LAUNCHER_APP_COMMON_H */
