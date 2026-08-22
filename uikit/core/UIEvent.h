#ifndef UIKIT_EVENT_H
#define UIKIT_EVENT_H

#include <stdint.h>

/**
 * @file UIEvent.h
 * @brief Target-action event system for UIKit.
 *
 * Provides a UIKit-style event abstraction on top of LVGL's event system.
 * Components register callbacks for specific event types, and the framework
 * translates LVGL events into UIEvent structures.
 */

typedef struct UIView UIView;   /* Forward declaration (UIView.h includes us) */

typedef enum {
    UIEVENT_TOUCH_DOWN     = 0,
    UIEVENT_TOUCH_UP       = 1,
    UIEVENT_CLICK          = 2,
    UIEVENT_LONG_PRESS     = 3,
    UIEVENT_VALUE_CHANGED  = 4,
    UIEVENT_FOCUS          = 5,
    UIEVENT_DEFOCUS        = 6,
    UIEVENT_KEY            = 7,
    UIEVENT_CUSTOM         = 8
} UIEventType;

typedef struct {
    UIEventType type;
    UIView *target;
    void *user_data;
    int32_t x;
    int32_t y;
    int32_t value;
} UIEvent;

/**
 * Callback function type for UI events.
 * @param event  The event data (temporary, do not retain).
 * @param user_data  User-provided context pointer.
 */
typedef void (*UIEventCallback)(UIEvent *event, void *user_data);

#endif /* UIKIT_EVENT_H */
