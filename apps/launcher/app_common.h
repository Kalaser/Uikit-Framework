/**
 * @file app_common.h
 * @brief Shared helpers for the standalone launcher apps.
 */

#ifndef LAUNCHER_APP_COMMON_H
#define LAUNCHER_APP_COMMON_H

#include "UIKit.hpp"
#include <cstdio>
#include <memory>

namespace launcher {

/** Print a lifecycle log line. */
inline void vc_log(const char *who, const char *cb)
{
    std::printf("[%s] %s\n", who, cb);
}

/** Defer destroy()+delete() of a popped controller until the next LVGL tick
 *  so the click callback can return before the object is freed. */
inline void schedule_delete(uikit::UIViewController *vc)
{
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
inline std::unique_ptr<uikit::UIButton> make_back(uikit::UIViewController *self,
                                                  uikit::UIView &parent,
                                                  uikit::UINavigationController *nav)
{
    auto btn = std::make_unique<uikit::UIButton>(&parent);
    btn->setTitle(LV_SYMBOL_LEFT " Back");
    btn->setTitleColor(0xFFFFFF);
    btn->setBackgroundColor(0x000000);
    btn->setOpacity(200);
    btn->setCornerRadius(22);
    btn->setFrame(20, 60, 130, 44);
    btn->onClick([self, nav](uikit::UIButton *) {
        if (nav) {
            nav->pop();
            launcher::schedule_delete(self);
        }
    });
    return btn;
}

/** Page title centered under the status bar. */
inline std::unique_ptr<uikit::UILabel> make_title(uikit::UIView &parent, const char *text)
{
    auto lbl = std::make_unique<uikit::UILabel>(&parent);
    lbl->setText(text);
    lbl->setColor(0xFFFFFF);
    lbl->setAlignment(1);
    lbl->setFont(&lv_font_montserrat_28);
    const int w = lv_disp_get_hor_res(NULL);
    lbl->setFrame(0, 30, w, 40);
    return lbl;
}

} /* namespace launcher */

#endif /* LAUNCHER_APP_COMMON_H */
