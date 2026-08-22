/**
 * @file UIKit.hpp
 * @brief C++ wrapper for UIKit-Framework (header-only, C++11).
 *
 * Design notes:
 *  - All classes live in namespace `uikit` to avoid clashing with the
 *    global C types (e.g. ::UIViewController).
 *  - Wrappers are move-only (RAII): each owns the underlying C object and
 *    destroys it on destruction. Copying is disabled.
 *  - Components with event callbacks (UIButton, UISwitch, UISlider,
 *    UITextField, UIPageControl) re-bind their C event user_data when moved.
 *  - UIViewController lifecycle callbacks map to virtual functions via
 *    static thunks (user_data == C++ `this`). onDestroy is only guaranteed
 *    to dispatch virtually while the object is alive — i.e. triggered by the
 *    navigation controller or an explicit destroy(); the destructor is a
 *    fallback only.
 *  - Ownership contract: controllers passed to UINavigationController must
 *    be heap-allocated (`new`); the nav takes ownership, triggers their
 *    onDestroy (virtual dispatch, object alive) and deletes them on
 *    destruction. pop() returns ownership to the caller.
 */

#ifndef UIKIT_CPP_HPP
#define UIKIT_CPP_HPP

#include <cstdint>
#include <functional>
#include <utility>
#include <vector>
#include <algorithm>

extern "C" {
#include "UIKit.h"
}

namespace uikit {

/* ══ UIView ───────────────────────────────────────────────────────────────── */

class UIView {
public:
    UIView() : m_view(::UIView_create(nullptr)), m_owned(true) {}
    explicit UIView(::UIView *v, bool owned = false) : m_view(v), m_owned(owned) {}
    /** Create a new owned view parented under `parent` (matches every other
     *  component's ctor). */
    explicit UIView(UIView *parent)
        : m_view(::UIView_create(parent ? parent->native() : nullptr)), m_owned(true) {}
    virtual ~UIView() { release(); }

    UIView(const UIView &) = delete;
    UIView &operator=(const UIView &) = delete;

    UIView(UIView &&o) noexcept : m_view(o.m_view), m_owned(o.m_owned) {
        o.m_view = nullptr;
        o.m_owned = false;
    }
    UIView &operator=(UIView &&o) noexcept {
        if (this != &o) {
            release();
            m_view = o.m_view;
            m_owned = o.m_owned;
            o.m_view = nullptr;
            o.m_owned = false;
        }
        return *this;
    }

    ::UIView *native() const { return m_view; }
    bool valid() const { return m_view != nullptr; }

    /* Frame / layout */
    void setFrame(int32_t x, int32_t y, int32_t w, int32_t h) { ::UIView_set_frame(m_view, x, y, w, h); }
    void getFrame(int32_t *x, int32_t *y, int32_t *w, int32_t *h) const { ::UIView_get_frame(m_view, x, y, w, h); }
    int32_t x() const { int32_t v = 0; ::UIView_get_frame(m_view, &v, nullptr, nullptr, nullptr); return v; }
    int32_t y() const { int32_t v = 0; ::UIView_get_frame(m_view, nullptr, &v, nullptr, nullptr); return v; }
    int32_t width() const { int32_t v = 0; ::UIView_get_frame(m_view, nullptr, nullptr, &v, nullptr); return v; }
    int32_t height() const { int32_t v = 0; ::UIView_get_frame(m_view, nullptr, nullptr, nullptr, &v); return v; }
    void setPosition(int32_t x, int32_t y) { ::UIView_set_position(m_view, x, y); }
    void setSize(int32_t w, int32_t h) { ::UIView_set_size(m_view, w, h); }

    /* Visibility */
    void setHidden(bool h) { ::UIView_set_hidden(m_view, h); }
    bool isHidden() const { return ::UIView_is_hidden(m_view); }

    /* Hierarchy */
    void addSubview(UIView &child) { ::UIView_add_subview(m_view, child.native()); }
    void removeFromSuperview() { ::UIView_remove_from_superview(m_view); }

    /* Appearance */
    void setBackgroundColor(uint32_t color) { ::UIView_set_background_color(m_view, color); }
    void setBorder(uint32_t color, int32_t width) { ::UIView_set_border(m_view, color, width); }
    void setCornerRadius(int32_t radius) { ::UIView_set_corner_radius(m_view, radius); }
    void setOpacity(uint8_t opacity) { ::UIView_set_opacity(m_view, opacity); }

    /* Interaction */
    void setUserInteraction(bool enabled) { ::UIView_set_user_interaction(m_view, enabled); }
    void setEnabled(bool enabled) { ::UIView_set_enabled(m_view, enabled); }
    bool isEnabled() const { return ::UIView_is_enabled(m_view); }

    /* Tag */
    void setTag(void *t) { ::UIView_set_tag(m_view, t); }
    void *tag() const { return ::UIView_get_tag(m_view); }

    /* Animation */
    void animate(::UIAnimationProperty prop, int32_t to, uint16_t ms) { ::UIView_animate(m_view, prop, to, ms); }
    void stopAnimations() { ::UIView_stop_animations(m_view); }

protected:
    ::UIView *m_view = nullptr;
    bool m_owned = false;

    void release() {
        if (m_owned && m_view) {
            ::UIView_destroy(m_view);
        }
        m_view = nullptr;
        m_owned = false;
    }
};

/* ══ UILabel ──────────────────────────────────────────────────────────────── */

class UILabel : public UIView {
public:
    UILabel() : UIView(reinterpret_cast<::UIView *>(::UILabel_create(nullptr)), true) {}
    explicit UILabel(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UILabel_create(parent ? parent->native() : nullptr)), true) {}
    static UILabel create(UIView *parent = nullptr) { return UILabel(parent); }

    void setText(const char *text) { ::UILabel_set_text(c(), text); }
    const char *text() const { return ::UILabel_get_text(c()); }
    void setColor(uint32_t color) { ::UILabel_set_color(c(), color); }
    void setAlignment(int32_t align) { ::UILabel_set_alignment(c(), align); }
    void setFont(const void *font) { ::UILabel_set_font(c(), font); }

private:
    ::UILabel *c() const { return reinterpret_cast<::UILabel *>(m_view); }
};

/* ══ UIButton ─────────────────────────────────────────────────────────────── */

class UIButton : public UIView {
public:
    UIButton() : UIView(reinterpret_cast<::UIView *>(::UIButton_create(nullptr)), true) {}
    explicit UIButton(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UIButton_create(parent ? parent->native() : nullptr)), true) {}
    static UIButton create(UIView *parent = nullptr) { return UIButton(parent); }

    UIButton(UIButton &&o) noexcept : UIView(std::move(o)) {
        m_on_click = std::move(o.m_on_click);
        o.m_on_click = nullptr;
        rebind_click();
    }
    UIButton &operator=(UIButton &&o) noexcept {
        if (this != &o) {
            ::UIView_remove_callback(m_view, UIEVENT_CLICK, &UIButton::click_thunk);
            UIView::operator=(std::move(o));
            m_on_click = std::move(o.m_on_click);
            o.m_on_click = nullptr;
            rebind_click();
        }
        return *this;
    }
    UIButton(const UIButton &) = delete;
    UIButton &operator=(const UIButton &) = delete;

    void setTitle(const char *title) { ::UIButton_set_title(c(), title); }
    const char *title() const { return ::UIButton_get_title(c()); }
    void setText(const char *text) { ::UIButton_set_text(c(), text); }
    void setTitleColor(uint32_t color) { ::UIButton_set_title_color(c(), color); }

    void onClick(std::function<void(UIButton *)> cb) {
        ::UIView_remove_callback(m_view, UIEVENT_CLICK, &UIButton::click_thunk);
        m_on_click = std::move(cb);
        if (m_on_click) {
            ::UIView_add_callback(m_view, UIEVENT_CLICK, &UIButton::click_thunk, this);
        }
    }

private:
    ::UIButton *c() const { return reinterpret_cast<::UIButton *>(m_view); }
    std::function<void(UIButton *)> m_on_click;

    static void click_thunk(UIEvent *ev, void *user_data) {
        (void)ev;
        auto *self = static_cast<UIButton *>(user_data);
        if (self && self->m_on_click) {
            self->m_on_click(self);
        }
    }
    void rebind_click() {
        ::UIView_remove_callback(m_view, UIEVENT_CLICK, &UIButton::click_thunk);
        if (m_on_click) {
            ::UIView_add_callback(m_view, UIEVENT_CLICK, &UIButton::click_thunk, this);
        }
    }
};

/* ══ UISwitch ─────────────────────────────────────────────────────────────── */

class UISwitch : public UIView {
public:
    UISwitch() : UIView(reinterpret_cast<::UIView *>(::UISwitch_create(nullptr)), true) {}
    explicit UISwitch(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UISwitch_create(parent ? parent->native() : nullptr)), true) {}
    static UISwitch create(UIView *parent = nullptr) { return UISwitch(parent); }

    UISwitch(UISwitch &&o) noexcept : UIView(std::move(o)) {
        m_on_change = std::move(o.m_on_change);
        o.m_on_change = nullptr;
        rebind_change();
    }
    UISwitch &operator=(UISwitch &&o) noexcept {
        if (this != &o) {
            ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UISwitch::change_thunk);
            UIView::operator=(std::move(o));
            m_on_change = std::move(o.m_on_change);
            o.m_on_change = nullptr;
            rebind_change();
        }
        return *this;
    }
    UISwitch(const UISwitch &) = delete;
    UISwitch &operator=(const UISwitch &) = delete;

    void setOn(bool on) { ::UISwitch_set_on(c(), on); }
    void setOnInstant(bool on) { ::UISwitch_set_on_instant(c(), on); }
    bool isOn() const { return ::UISwitch_is_on(c()); }
    void setKnobColor(uint32_t color) { ::UISwitch_set_knob_color(c(), color); }
    void setOnColor(uint32_t color) { ::UISwitch_set_on_color(c(), color); }

    void onChange(std::function<void(bool)> cb) {
        ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UISwitch::change_thunk);
        m_on_change = std::move(cb);
        if (m_on_change) {
            ::UIView_add_callback(m_view, UIEVENT_VALUE_CHANGED, &UISwitch::change_thunk, this);
        }
    }

private:
    ::UISwitch *c() const { return reinterpret_cast<::UISwitch *>(m_view); }
    std::function<void(bool)> m_on_change;

    static void change_thunk(UIEvent *ev, void *user_data) {
        (void)ev;
        auto *self = static_cast<UISwitch *>(user_data);
        if (self && self->m_on_change) {
            self->m_on_change(self->isOn());
        }
    }
    void rebind_change() {
        ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UISwitch::change_thunk);
        if (m_on_change) {
            ::UIView_add_callback(m_view, UIEVENT_VALUE_CHANGED, &UISwitch::change_thunk, this);
        }
    }
};

/* ══ UISlider ─────────────────────────────────────────────────────────────── */

class UISlider : public UIView {
public:
    UISlider() : UIView(reinterpret_cast<::UIView *>(::UISlider_create(nullptr)), true) {}
    explicit UISlider(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UISlider_create(parent ? parent->native() : nullptr)), true) {}
    static UISlider create(UIView *parent = nullptr) { return UISlider(parent); }

    UISlider(UISlider &&o) noexcept : UIView(std::move(o)) {
        m_on_change = std::move(o.m_on_change);
        o.m_on_change = nullptr;
        rebind_change();
    }
    UISlider &operator=(UISlider &&o) noexcept {
        if (this != &o) {
            ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UISlider::change_thunk);
            UIView::operator=(std::move(o));
            m_on_change = std::move(o.m_on_change);
            o.m_on_change = nullptr;
            rebind_change();
        }
        return *this;
    }
    UISlider(const UISlider &) = delete;
    UISlider &operator=(const UISlider &) = delete;

    void setRange(int32_t min, int32_t max) { ::UISlider_set_range(c(), min, max); }
    void setValue(int32_t value) { ::UISlider_set_value(c(), value); }
    int32_t value() const { return ::UISlider_get_value(c()); }
    void setVertical(bool vertical) { ::UISlider_set_vertical(c(), vertical); }
    void setIndicatorColor(uint32_t color) { ::UISlider_set_indicator_color(c(), color); }
    void setKnobColor(uint32_t color) { ::UISlider_set_knob_color(c(), color); }

    void onChange(std::function<void(int)> cb) {
        ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UISlider::change_thunk);
        m_on_change = std::move(cb);
        if (m_on_change) {
            ::UIView_add_callback(m_view, UIEVENT_VALUE_CHANGED, &UISlider::change_thunk, this);
        }
    }

private:
    ::UISlider *c() const { return reinterpret_cast<::UISlider *>(m_view); }
    std::function<void(int)> m_on_change;

    static void change_thunk(UIEvent *ev, void *user_data) {
        (void)ev;
        auto *self = static_cast<UISlider *>(user_data);
        if (self && self->m_on_change) {
            self->m_on_change(self->value());
        }
    }
    void rebind_change() {
        ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UISlider::change_thunk);
        if (m_on_change) {
            ::UIView_add_callback(m_view, UIEVENT_VALUE_CHANGED, &UISlider::change_thunk, this);
        }
    }
};

/* ══ UITextField ──────────────────────────────────────────────────────────── */

class UITextField : public UIView {
public:
    UITextField() : UIView(reinterpret_cast<::UIView *>(::UITextField_create(nullptr)), true) {}
    explicit UITextField(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UITextField_create(parent ? parent->native() : nullptr)), true) {}
    static UITextField create(UIView *parent = nullptr) { return UITextField(parent); }

    UITextField(UITextField &&o) noexcept : UIView(std::move(o)) {
        m_on_change = std::move(o.m_on_change);
        o.m_on_change = nullptr;
        rebind_change();
    }
    UITextField &operator=(UITextField &&o) noexcept {
        if (this != &o) {
            ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UITextField::change_thunk);
            UIView::operator=(std::move(o));
            m_on_change = std::move(o.m_on_change);
            o.m_on_change = nullptr;
            rebind_change();
        }
        return *this;
    }
    UITextField(const UITextField &) = delete;
    UITextField &operator=(const UITextField &) = delete;

    void setText(const char *text) { ::UITextField_set_text(c(), text); }
    const char *text() const { return ::UITextField_get_text(c()); }
    void setPlaceholder(const char *text) { ::UITextField_set_placeholder(c(), text); }
    void setTextColor(uint32_t color) { ::UITextField_set_text_color(c(), color); }
    void setMaxLength(uint32_t len) { ::UITextField_set_max_length(c(), len); }
    void setPasswordMode(bool pw) { ::UITextField_set_password_mode(c(), pw); }
    void setEditable(bool editable) { ::UITextField_set_editable(c(), editable); }

    void onChange(std::function<void(const char *)> cb) {
        ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UITextField::change_thunk);
        m_on_change = std::move(cb);
        if (m_on_change) {
            ::UIView_add_callback(m_view, UIEVENT_VALUE_CHANGED, &UITextField::change_thunk, this);
        }
    }

private:
    ::UITextField *c() const { return reinterpret_cast<::UITextField *>(m_view); }
    std::function<void(const char *)> m_on_change;

    static void change_thunk(UIEvent *ev, void *user_data) {
        (void)ev;
        auto *self = static_cast<UITextField *>(user_data);
        if (self && self->m_on_change) {
            self->m_on_change(self->text());
        }
    }
    void rebind_change() {
        ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UITextField::change_thunk);
        if (m_on_change) {
            ::UIView_add_callback(m_view, UIEVENT_VALUE_CHANGED, &UITextField::change_thunk, this);
        }
    }
};

/* ══ UIImageView ──────────────────────────────────────────────────────────── */

class UIImageView : public UIView {
public:
    UIImageView() : UIView(reinterpret_cast<::UIView *>(::UIImageView_create(nullptr)), true) {}
    explicit UIImageView(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UIImageView_create(parent ? parent->native() : nullptr)), true) {}
    static UIImageView create(UIView *parent = nullptr) { return UIImageView(parent); }

    void setImage(const void *src) { ::UIImageView_set_image(c(), src); }
    void setScale(uint16_t scale) { ::UIImageView_set_scale(c(), scale); }
    void setRotation(int16_t angle) { ::UIImageView_set_rotation(c(), angle); }
    void setAntialias(bool antialias) { ::UIImageView_set_antialias(c(), antialias); }

private:
    ::UIImageView *c() const { return reinterpret_cast<::UIImageView *>(m_view); }
};

/* ══ UIScrollView ─────────────────────────────────────────────────────────── */

class UIScrollView : public UIView {
public:
    UIScrollView() : UIView(reinterpret_cast<::UIView *>(::UIScrollView_create(nullptr)), true) {}
    explicit UIScrollView(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UIScrollView_create(parent ? parent->native() : nullptr)), true) {}
    static UIScrollView create(UIView *parent = nullptr) { return UIScrollView(parent); }

    void setVerticalScroll(bool enabled) { ::UIScrollView_set_vertical_scroll(c(), enabled); }
    void setHorizontalScroll(bool enabled) { ::UIScrollView_set_horizontal_scroll(c(), enabled); }
    void setScrollbarVisible(bool visible) { ::UIScrollView_set_scrollbar_visible(c(), visible); }
    void scrollToView(UIView &subview) { ::UIScrollView_scroll_to_view(c(), subview.native()); }
    void scrollTo(int32_t x, int32_t y) { ::UIScrollView_scroll_to(c(), x, y); }

private:
    ::UIScrollView *c() const { return reinterpret_cast<::UIScrollView *>(m_view); }
};

/* ══ UIProgressView ───────────────────────────────────────────────────────── */

class UIProgressView : public UIView {
public:
    UIProgressView() : UIView(reinterpret_cast<::UIView *>(::UIProgressView_create(nullptr)), true) {}
    explicit UIProgressView(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UIProgressView_create(parent ? parent->native() : nullptr)), true) {}
    static UIProgressView create(UIView *parent = nullptr) { return UIProgressView(parent); }

    void setRange(int32_t min, int32_t max) { ::UIProgressView_set_range(c(), min, max); }
    void setValue(int32_t value) { ::UIProgressView_set_value(c(), value); }
    void setValueInstant(int32_t value) { ::UIProgressView_set_value_instant(c(), value); }
    int32_t value() const { return ::UIProgressView_get_value(c()); }
    void setTrackColor(uint32_t color) { ::UIProgressView_set_track_color(c(), color); }
    void setProgressColor(uint32_t color) { ::UIProgressView_set_progress_color(c(), color); }
    void setVertical(bool vertical) { ::UIProgressView_set_vertical(c(), vertical); }

private:
    ::UIProgressView *c() const { return reinterpret_cast<::UIProgressView *>(m_view); }
};

/* ══ UIPageControl ────────────────────────────────────────────────────────── */

class UIPageControl : public UIView {
public:
    UIPageControl() : UIView(reinterpret_cast<::UIView *>(::UIPageControl_create(nullptr)), true) {}
    explicit UIPageControl(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UIPageControl_create(parent ? parent->native() : nullptr)), true) {}
    static UIPageControl create(UIView *parent = nullptr) { return UIPageControl(parent); }

    UIPageControl(UIPageControl &&o) noexcept : UIView(std::move(o)) {
        m_on_change = std::move(o.m_on_change);
        o.m_on_change = nullptr;
        rebind_change();
    }
    UIPageControl &operator=(UIPageControl &&o) noexcept {
        if (this != &o) {
            ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UIPageControl::change_thunk);
            UIView::operator=(std::move(o));
            m_on_change = std::move(o.m_on_change);
            o.m_on_change = nullptr;
            rebind_change();
        }
        return *this;
    }
    UIPageControl(const UIPageControl &) = delete;
    UIPageControl &operator=(const UIPageControl &) = delete;

    void setPageCount(uint32_t count) { ::UIPageControl_set_page_count(c(), count); }
    uint32_t pageCount() const { return ::UIPageControl_get_page_count(c()); }
    void setCurrentPage(uint32_t page) { ::UIPageControl_set_current_page(c(), page); }
    uint32_t currentPage() const { return ::UIPageControl_get_current_page(c()); }
    void setCurrentColor(uint32_t color) { ::UIPageControl_set_current_color(c(), color); }
    void setPageColor(uint32_t color) { ::UIPageControl_set_page_color(c(), color); }

    void onChange(std::function<void(uint32_t)> cb) {
        ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UIPageControl::change_thunk);
        m_on_change = std::move(cb);
        if (m_on_change) {
            ::UIView_add_callback(m_view, UIEVENT_VALUE_CHANGED, &UIPageControl::change_thunk, this);
        }
    }

private:
    ::UIPageControl *c() const { return reinterpret_cast<::UIPageControl *>(m_view); }
    std::function<void(uint32_t)> m_on_change;

    static void change_thunk(UIEvent *ev, void *user_data) {
        (void)ev;
        auto *self = static_cast<UIPageControl *>(user_data);
        if (self && self->m_on_change) {
            self->m_on_change(self->currentPage());
        }
    }
    void rebind_change() {
        ::UIView_remove_callback(m_view, UIEVENT_VALUE_CHANGED, &UIPageControl::change_thunk);
        if (m_on_change) {
            ::UIView_add_callback(m_view, UIEVENT_VALUE_CHANGED, &UIPageControl::change_thunk, this);
        }
    }
};

/* ══ UIStackView ──────────────────────────────────────────────────────────── */

class UIStackView : public UIView {
public:
    UIStackView() : UIView(reinterpret_cast<::UIView *>(::UIStackView_create(nullptr)), true) {}
    explicit UIStackView(UIView *parent)
        : UIView(reinterpret_cast<::UIView *>(::UIStackView_create(parent ? parent->native() : nullptr)), true) {}
    static UIStackView create(UIView *parent = nullptr) { return UIStackView(parent); }

    void setAxis(::UIStackAxis axis) { ::UIStackView_set_axis(c(), axis); }
    void setSpacing(int32_t spacing) { ::UIStackView_set_spacing(c(), spacing); }
    void setPadding(int32_t padding) { ::UIStackView_set_padding(c(), padding); }
    void addArrangedSubview(UIView &child) { ::UIStackView_add_arranged_subview(c(), child.native()); }
    void setAlignment(int32_t align) { ::UIStackView_set_alignment(c(), align); }

private:
    ::UIStackView *c() const { return reinterpret_cast<::UIStackView *>(m_view); }
};

/* ══ UIViewController ─────────────────────────────────────────────────────── */

class UIViewController {
public:
    UIViewController()
        : m_vc(static_cast<::UIViewController *>(::UIKit_calloc(1, sizeof(::UIViewController)))) {
        ::UIViewController_init(m_vc, nullptr);
        m_vc->user_data = this;
        m_vc->onCreate  = &UIViewController::thunk_create;
        m_vc->onStart   = &UIViewController::thunk_start;
        m_vc->onResume  = &UIViewController::thunk_resume;
        m_vc->onPause   = &UIViewController::thunk_pause;
        m_vc->onStop    = &UIViewController::thunk_stop;
        m_vc->onDestroy = &UIViewController::thunk_destroy;
    }
    virtual ~UIViewController() {
        if (m_vc) {
            ::UIViewController_destroy(m_vc);   /* fallback: frees C struct */
            m_vc = nullptr;
        }
    }

    UIViewController(const UIViewController &) = delete;
    UIViewController &operator=(const UIViewController &) = delete;

    UIViewController(UIViewController &&o) noexcept : m_vc(o.m_vc) {
        o.m_vc = nullptr;
        if (m_vc) {
            m_vc->user_data = this;
        }
    }
    UIViewController &operator=(UIViewController &&o) noexcept {
        if (this != &o) {
            if (m_vc) {
                ::UIViewController_destroy(m_vc);
            }
            m_vc = o.m_vc;
            o.m_vc = nullptr;
            if (m_vc) {
                m_vc->user_data = this;
            }
        }
        return *this;
    }

    /* Lifecycle — override in subclasses */
    virtual void onCreate() {}
    virtual void onStart() {}
    virtual void onResume() {}
    virtual void onPause() {}
    virtual void onStop() {}
    virtual void onDestroy() {}

    /* View (borrowed — the controller owns it) */
    UIView getView() { return UIView(::UIViewController_get_view(m_vc), false); }

    void setTitle(const char *title) { ::UIViewController_set_title(m_vc, title); }
    const char *title() const { return ::UIViewController_get_title(m_vc); }

    ::UIViewController *c_ptr() { return m_vc; }
    ::UIViewControllerState state() const { return m_vc ? m_vc->state : ::UIViewControllerState_None; }

    /**
     * Explicit teardown while the object is alive: fires onDestroy()
     * through the normal virtual dispatch, then releases the C struct.
     * After this the wrapper is inert (c_ptr() == nullptr).
     */
    void destroy() {
        if (m_vc) {
            ::UIViewController_destroy(m_vc);
            m_vc = nullptr;
        }
    }

    /** Internal: drop the C pointer (used by the nav after it freed it). */
    void detach_c() { m_vc = nullptr; }

protected:
    ::UIViewController *m_vc = nullptr;

    static void thunk_create(::UIViewController *vc) {
        static_cast<UIViewController *>(vc->user_data)->onCreate();
    }
    static void thunk_start(::UIViewController *vc) {
        static_cast<UIViewController *>(vc->user_data)->onStart();
    }
    static void thunk_resume(::UIViewController *vc) {
        static_cast<UIViewController *>(vc->user_data)->onResume();
    }
    static void thunk_pause(::UIViewController *vc) {
        static_cast<UIViewController *>(vc->user_data)->onPause();
    }
    static void thunk_stop(::UIViewController *vc) {
        static_cast<UIViewController *>(vc->user_data)->onStop();
    }
    static void thunk_destroy(::UIViewController *vc) {
        static_cast<UIViewController *>(vc->user_data)->onDestroy();
    }
};

/* ══ UINavigationController ───────────────────────────────────────────────── */

class UINavigationController {
public:
    /**
     * Takes ownership of `root` (must be heap-allocated with new).
     */
    explicit UINavigationController(UIViewController *root) : m_nav(nullptr) {
        if (!root) {
            return;
        }
        m_nav = ::UINavigationController_create(root->c_ptr());
        if (m_nav) {
            m_owned.push_back(root);
        }
    }
    ~UINavigationController() {
        /* 1. C layer teardown: fires onDestroy on every stacked controller
         *    (objects alive → virtual dispatch works), frees C structs. */
        if (m_nav) {
            ::UINavigationController_destroy(m_nav);
            m_nav = nullptr;
        }
        /* 2. Free the C++ objects (their C structs are already freed). */
        for (auto *vc : m_owned) {
            vc->detach_c();
            delete vc;
        }
        m_owned.clear();
    }

    UINavigationController(const UINavigationController &) = delete;
    UINavigationController &operator=(const UINavigationController &) = delete;

    /**
     * Push `vc` — ownership transfers to the nav (must be heap-allocated).
     */
    void push(UIViewController *vc) {
        if (!vc || !m_nav) {
            return;
        }
        ::UINavigationController_push(m_nav, vc->c_ptr());
        m_owned.push_back(vc);
    }

    /**
     * Pop the top controller — ownership returns to the caller.
     * @return The popped controller (delete it when done, or push again),
     *         or nullptr if only the root remains.
     */
    UIViewController *pop() {
        if (!m_nav) {
            return nullptr;
        }
        ::UIViewController *c = ::UINavigationController_pop(m_nav);
        if (!c) {
            return nullptr;
        }
        auto *cpp = static_cast<UIViewController *>(c->user_data);
        auto it = std::find(m_owned.begin(), m_owned.end(), cpp);
        if (it != m_owned.end()) {
            m_owned.erase(it);
        }
        return cpp;
    }

    /**
     * Pop everything except the root controller.
     * @return The popped controllers (caller owns them — delete when done),
     *         in pop order (top first). Empty if only the root remains.
     */
    std::vector<UIViewController *> popToRoot() {
        std::vector<UIViewController *> out;
        if (!m_nav) {
            return out;
        }
        int n = ::UINavigationController_pop_to_root(m_nav);
        while (n > 0 && m_owned.size() > 1) {
            UIViewController *vc = m_owned.back();
            m_owned.pop_back();
            out.push_back(vc);
            n--;
        }
        return out;
    }

    int count() const { return m_nav ? ::UINavigationController_count(m_nav) : 0; }
    UIViewController *top() const {
        if (!m_nav) {
            return nullptr;
        }
        ::UIViewController *c = ::UINavigationController_top(m_nav);
        return c ? static_cast<UIViewController *>(c->user_data) : nullptr;
    }
    UIViewController *at(int index) const {
        if (!m_nav) {
            return nullptr;
        }
        ::UIViewController *c = ::UINavigationController_at(m_nav, index);
        return c ? static_cast<UIViewController *>(c->user_data) : nullptr;
    }
    void setAnimated(bool animated) { ::UINavigationController_set_animated(m_nav, animated); }

    ::UINavigationController *c_ptr() { return m_nav; }

private:
    ::UINavigationController *m_nav;
    std::vector<UIViewController *> m_owned;
};

} /* namespace uikit */

#endif /* UIKIT_CPP_HPP */
