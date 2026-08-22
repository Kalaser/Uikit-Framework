/**
 * @file win_driver.c
 * @brief Windows GDI display + input backend for LVGL (16bpp RGB565).
 */

#include "win_driver.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define WIN_W 480
#define WIN_H 800
#define WIN_MIN_W 320
#define WIN_MIN_H 480
/* Max supported client width — the LVGL draw buffer must hold at least one
 * full row; we size it for up to 1920 px wide windows. */
#define WIN_MAX_W 1920

typedef struct {
    HWND hwnd;
    HDC  hdc_mem;      /* Back buffer DC (owns the DIB) */
    HBITMAP hbmp;      /* DIB section, 16bpp top-down */
    void *bits;        /* DIB pixel memory */
    int width;         /* Current client width (DIB stride = width*2) */
    int height;        /* Current client height */

    int mouse_x, mouse_y;
    int mouse_down;

    uint32_t key;      /* Pending keypad key for LVGL */

    int smoke_frames;
    int frame_count;
    DWORD last_tick;
} WinDriver;

static WinDriver s_wd;
static lv_disp_draw_buf_t s_draw_buf;
static lv_color_t s_buf1[WIN_MAX_W * 60];   /* partial mode render buffer */
static lv_disp_drv_t s_disp_drv;
static lv_indev_drv_t s_indev_drv;
static lv_indev_drv_t s_kb_drv;

static void (*s_resize_cb)(int w, int h) = NULL;

int g_win_width  = WIN_W;
int g_win_height = WIN_H;

/* ── LVGL callbacks ───────────────────────────────────────────────────────── */

static void win_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    WinDriver *wd = (WinDriver *)drv->user_data;

    if (wd->hwnd && wd->hdc_mem && wd->bits) {
        int w = lv_area_get_width(area);
        int h = lv_area_get_height(area);

        /* Copy the rendered area into the back-buffer DIB (same RGB565 layout) */
        for (int y = 0; y < h; y++) {
            memcpy((unsigned char *)wd->bits + (area->y1 + y) * wd->width * 2 + area->x1 * 2,
                   (unsigned char *)color_p + y * w * 2, (size_t)w * 2);
        }
        /* Flip the area to the window */
        HDC hdc = GetDC(wd->hwnd);
        BitBlt(hdc, area->x1, area->y1, w, h, wd->hdc_mem, area->x1, area->y1, SRCCOPY);
        ReleaseDC(wd->hwnd, hdc);
    }
    lv_disp_flush_ready(drv);
}

static void win_indev_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    WinDriver *wd = (WinDriver *)drv->user_data;
    data->point.x = wd->mouse_x;
    data->point.y = wd->mouse_y;
    data->state = wd->mouse_down ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

static void win_kb_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    WinDriver *wd = (WinDriver *)drv->user_data;
    if (wd->key != 0) {
        data->key = wd->key;
        wd->key = 0;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

/* ── Resize handling ──────────────────────────────────────────────────────── */

static void win_rebuild_backbuffer(int w, int h)
{
    if (s_wd.hdc_mem) {
        DeleteDC(s_wd.hdc_mem);
        s_wd.hdc_mem = NULL;
    }
    if (s_wd.hbmp) {
        DeleteObject(s_wd.hbmp);
        s_wd.hbmp = NULL;
    }
    s_wd.bits = NULL;

    if (w <= 0 || h <= 0) {
        return;
    }

    struct { BITMAPINFOHEADER h; DWORD masks[3]; } bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.h.biSize = sizeof(BITMAPINFOHEADER);
    bmi.h.biWidth = w;
    bmi.h.biHeight = -h;        /* top-down: row 0 at top */
    bmi.h.biPlanes = 1;
    bmi.h.biBitCount = 16;
    bmi.h.biCompression = BI_BITFIELDS;
    bmi.masks[0] = 0xF800;      /* R5 */
    bmi.masks[1] = 0x07E0;      /* G6 */
    bmi.masks[2] = 0x001F;      /* B5 */

    HDC screen = GetDC(NULL);
    s_wd.hbmp = CreateDIBSection(screen, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, &s_wd.bits, NULL, 0);
    ReleaseDC(NULL, screen);
    if (!s_wd.hbmp) {
        return;
    }
    s_wd.hdc_mem = CreateCompatibleDC(NULL);
    if (!s_wd.hdc_mem) {
        DeleteObject(s_wd.hbmp);
        s_wd.hbmp = NULL;
        s_wd.bits = NULL;
        return;
    }
    SelectObject(s_wd.hdc_mem, s_wd.hbmp);

    s_wd.width = w;
    s_wd.height = h;
    g_win_width = w;
    g_win_height = h;
}

static void win_handle_resize(int w, int h)
{
    if (w <= 0 || h <= 0) {
        return;
    }
    if (w == s_wd.width && h == s_wd.height) {
        return;
    }

    /* 1. Rebuild the GDI back buffer at the new size. */
    win_rebuild_backbuffer(w, h);

    /* 2. Update LVGL's resolution and force a full repaint. */
    lv_disp_t *disp = lv_disp_get_default();
    if (disp && disp->driver) {
        disp->driver->hor_res = w;
        disp->driver->ver_res = h;
        lv_disp_drv_update(disp, disp->driver);
        lv_obj_invalidate(lv_scr_act());
    }

    /* 3. Let the application re-layout. */
    if (s_resize_cb) {
        s_resize_cb(w, h);
    }
}

/* ── Window proc ──────────────────────────────────────────────────────────── */

static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    WinDriver *wd = &s_wd;
    switch (msg) {
    case WM_MOUSEMOVE:
        wd->mouse_x = (short)LOWORD(lp);
        wd->mouse_y = (short)HIWORD(lp);
        break;
    case WM_LBUTTONDOWN:
        SetCapture(hwnd);
        wd->mouse_x = (short)LOWORD(lp);
        wd->mouse_y = (short)HIWORD(lp);
        wd->mouse_down = 1;
        break;
    case WM_LBUTTONUP:
        wd->mouse_down = 0;
        ReleaseCapture();
        break;

    /* Keyboard: text characters + navigation keys */
    case WM_CHAR:
        wd->key = (uint32_t)wp;
        break;
    case WM_KEYDOWN:
        switch (wp) {
        case VK_BACK:   wd->key = LV_KEY_BACKSPACE; break;
        case VK_RETURN: wd->key = LV_KEY_ENTER;     break;
        case VK_TAB:    wd->key = LV_KEY_NEXT;      break;
        case VK_LEFT:   wd->key = LV_KEY_PREV;      break;
        case VK_RIGHT:  wd->key = LV_KEY_NEXT;      break;
        case VK_UP:     wd->key = LV_KEY_PREV;      break;
        case VK_DOWN:   wd->key = LV_KEY_NEXT;      break;
        default: break;
        }
        break;

    case WM_TIMER: {
        DWORD now = GetTickCount();
        DWORD dt = now - wd->last_tick;
        wd->last_tick = now;
        if (dt == 0) dt = 1;
        if (dt > 100) dt = 100;
        lv_tick_inc(dt);
        lv_timer_handler();

        if (wd->smoke_frames > 0) {
            wd->frame_count++;
            if (wd->frame_count >= wd->smoke_frames) {
                PostQuitMessage(0);
            }
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (wd->hdc_mem) {
            BitBlt(hdc, 0, 0, wd->width, wd->height, wd->hdc_mem, 0, 0, SRCCOPY);
        }
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_SIZE:
        win_handle_resize((int)(short)LOWORD(lp), (int)(short)HIWORD(lp));
        break;
    case WM_GETMINMAXINFO: {
        MINMAXINFO *mmi = (MINMAXINFO *)lp;
        RECT rc = { 0, 0, WIN_MIN_W, WIN_MIN_H };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        mmi->ptMinTrackSize.x = rc.right - rc.left;
        mmi->ptMinTrackSize.y = rc.bottom - rc.top;
        break;
    }
    case WM_ERASEBKGND:
        return 1;   /* prevent flicker */
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

/* ── Public API ───────────────────────────────────────────────────────────── */

int WinDriver_init(int smoke_frames)
{
    memset(&s_wd, 0, sizeof(s_wd));
    s_wd.smoke_frames = smoke_frames;
    s_wd.last_tick = GetTickCount();

    /* Window class */
    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "UIKitLauncherClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    if (!RegisterClass(&wc)) {
        return -1;
    }

    /* Client area sized to WIN_W x WIN_H */
    RECT rc = { 0, 0, WIN_W, WIN_H };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    s_wd.hwnd = CreateWindow("UIKitLauncherClass", "UIKit Launcher",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             rc.right - rc.left, rc.bottom - rc.top,
                             NULL, NULL, wc.hInstance, NULL);
    if (!s_wd.hwnd) {
        return -1;
    }

    /* Back buffer: 16bpp RGB565 top-down DIB section (rebuilt on resize) */
    s_wd.width = WIN_W;
    s_wd.height = WIN_H;
    win_rebuild_backbuffer(WIN_W, WIN_H);
    if (!s_wd.hbmp || !s_wd.hdc_mem) {
        return -1;
    }

    /* LVGL display: standard partial rendering (classic embedded setup —
     * full_refresh double-buffering produced unreliable output here).
     * The draw buffer is sized for the max supported width; LVGL renders
     * the screen in vertical chunks that fit the current resolution. */
    lv_disp_draw_buf_init(&s_draw_buf, s_buf1, NULL, WIN_MAX_W * 60);
    lv_disp_drv_init(&s_disp_drv);
    s_disp_drv.draw_buf = &s_draw_buf;
    s_disp_drv.hor_res = WIN_W;
    s_disp_drv.ver_res = WIN_H;
    s_disp_drv.flush_cb = win_flush_cb;
    s_disp_drv.user_data = &s_wd;
    lv_disp_drv_register(&s_disp_drv);

    /* LVGL pointer (mouse) */
    lv_indev_drv_init(&s_indev_drv);
    s_indev_drv.type = LV_INDEV_TYPE_POINTER;
    s_indev_drv.read_cb = win_indev_read_cb;
    s_indev_drv.user_data = &s_wd;
    lv_indev_drv_register(&s_indev_drv);

    /* LVGL keypad (Windows keyboard → text input) */
    lv_indev_drv_init(&s_kb_drv);
    s_kb_drv.type = LV_INDEV_TYPE_KEYPAD;
    s_kb_drv.read_cb = win_kb_read_cb;
    s_kb_drv.user_data = &s_wd;
    lv_indev_drv_register(&s_kb_drv);

    ShowWindow(s_wd.hwnd, SW_SHOW);
    UpdateWindow(s_wd.hwnd);
    SetTimer(s_wd.hwnd, 1, 16, NULL);   /* ~60 fps */

    return 0;
}

void WinDriver_run(void)
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WinDriver_set_resize_callback(void (*cb)(int w, int h))
{
    s_resize_cb = cb;
}

void WinDriver_deinit(void)
{
    if (s_wd.hwnd) {
        KillTimer(s_wd.hwnd, 1);
        DestroyWindow(s_wd.hwnd);
        s_wd.hwnd = NULL;
    }
    if (s_wd.hdc_mem) {
        DeleteDC(s_wd.hdc_mem);
        s_wd.hdc_mem = NULL;
    }
    if (s_wd.hbmp) {
        DeleteObject(s_wd.hbmp);
        s_wd.hbmp = NULL;
    }
    UnregisterClass("UIKitLauncherClass", GetModuleHandle(NULL));
}
