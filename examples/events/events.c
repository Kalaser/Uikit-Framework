/**
 * Example: Events — target-action event model (README §4 "events").
 *
 * Demonstrates button clicks, switch changes and slider value changes,
 * all through the framework-level event abstraction (UIEvent).
 */

#include "UIKit.h"
#include <stdio.h>

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[320 * 20];
static lv_disp_drv_t disp_drv;

static void flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    (void)drv; (void)area; (void)color_p;
    lv_disp_flush_ready(drv);
}

static void setup_display(void)
{
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, 320 * 20);
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res  = 320;
    disp_drv.ver_res  = 240;
    disp_drv.flush_cb = flush_cb;
    lv_disp_drv_register(&disp_drv);
}

static void on_button_event(UIEvent *event, void *user_data)
{
    (void)user_data;
    switch (event->type) {
    case UIEVENT_TOUCH_DOWN:  printf("button pressed\n");   break;
    case UIEVENT_TOUCH_UP:    printf("button released\n");  break;
    case UIEVENT_CLICK:       printf("button clicked\n");   break;
    default: break;
    }
}

static void on_switch_event(UIEvent *event, void *user_data)
{
    (void)user_data;
    printf("switch changed\n");
}

static void on_slider_event(UIEvent *event, void *user_data)
{
    (void)user_data;
    if (event->type == UIEVENT_VALUE_CHANGED) {
        printf("slider value: %d\n", event->value);
    }
}

int main(void)
{
    lv_init();
    setup_display();
    UIKit_init();

    UIView *root = UIViewRoot_create();

    UIButton *button = UIButton_create(root);
    UIButton_set_title(button, "Press me");
    UIView_set_frame(&button->base, 40, 40, 240, 50);
    UIButton_set_event_callback(button, UIEVENT_TOUCH_DOWN, on_button_event, NULL);
    UIButton_set_event_callback(button, UIEVENT_TOUCH_UP,   on_button_event, NULL);
    UIButton_on_click(button, on_button_event, NULL);

    UISwitch *sw = UISwitch_create(root);
    UIView_set_position(&sw->base, 40, 110);
    UISwitch_on_change(sw, on_switch_event, NULL);

    UISlider *slider = UISlider_create(root);
    UISlider_set_range(slider, 0, 100);
    UIView_set_frame(&slider->base, 40, 150, 240, 10);
    UISlider_on_change(slider, on_slider_event, NULL);

    /* Drive a few events programmatically */
    lv_event_send(button->base.lv_obj, LV_EVENT_PRESSED, NULL);
    lv_event_send(button->base.lv_obj, LV_EVENT_RELEASED, NULL);
    lv_event_send(button->base.lv_obj, LV_EVENT_SHORT_CLICKED, NULL);
    lv_event_send(sw->base.lv_obj, LV_EVENT_VALUE_CHANGED, NULL);
    lv_event_send(slider->base.lv_obj, LV_EVENT_VALUE_CHANGED, NULL);

    for (int i = 0; i < 20; i++) {
        lv_tick_inc(5);
        lv_timer_handler();
        UIKit_process();
    }

    printf("Events example finished.\n");
    return 0;
}
