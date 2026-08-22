/**
 * Example: Basic — minimum UIKit app (README §4 "basic").
 *
 * Shows a root view, a label and a button, and runs the LVGL loop.
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

static void on_start_clicked(UIEvent *event, void *user_data)
{
    (void)user_data;
    if (event->type == UIEVENT_CLICK) {
        printf("Start clicked\n");
    }
}

int main(void)
{
    lv_init();
    setup_display();
    UIKit_init();

    UIView *root = UIViewRoot_create();
    UIView_set_background_color(root, 0xF2F2F7);

    UILabel *title = UILabel_create(root);
    UILabel_set_text(title, "UIKit Basic");
    UILabel_set_alignment(title, 1);
    UIView_set_frame(&title->base, 0, 20, 320, 30);

    UIButton *button = UIButton_create(root);
    UIButton_set_text(button, "Start");
    UIButton_set_action(button, on_start_clicked, NULL);
    UIView_set_frame(&button->base, 40, 80, 240, 50);

    for (int i = 0; i < 20; i++) {
        lv_tick_inc(5);
        lv_timer_handler();
        UIKit_process();
    }

    printf("Basic example finished.\n");
    return 0;
}
