#pragma once
#include "foundation/basic.h"
#include "renderer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct Frame_Buffer {
    BITMAPINFO info;
    uint32_t *data;
    int width;
    int height;
} Frame_Buffer;

struct App {
    int window_w;
    int window_h;
    float render_scale;
    Frame_Buffer frame_buffer;
    bool running;
} *app = &(struct App) {
    .window_w = 800,
    .window_h = 600,
    .render_scale = 1.0f,
};

static void resize_dib_section(Frame_Buffer *buffer, int width, int height)
{
    if (buffer->data) {
        VirtualFree(buffer->data, 0, MEM_RELEASE);
    }

    buffer->width = width;
    buffer->height = height;

    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = buffer->width;
    buffer->info.bmiHeader.biHeight = -buffer->height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;

    const uint32_t total_bytes = buffer->width * buffer->height * sizeof(uint32_t);
    buffer->data = VirtualAlloc(0, total_bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    memset(buffer->data, 0, buffer->width * buffer->height);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = 0;
    switch (msg) {
        case WM_CLOSE: {
            app->running = false;
            break;
        }
        default: 
            lResult = DefWindowProc(hwnd, msg, wParam, lParam);
            break;
    }
    return lResult;
}

static uint64_t os_time_now()
{
    uint64_t now;
    LARGE_INTEGER qpc;
    QueryPerformanceCounter(&qpc);
    now = (uint64_t)qpc.QuadPart;
    return now;
}

static double os_time_delta(uint64_t to, uint64_t from)
{
    double result = 0.0;
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    result = (to - from) * (1.0 / freq.QuadPart);
    return result;
}

int main(int argc, char **argv)
{
    WNDCLASSA wc = { 
        0, WndProc, 0, 0, 0, 
        LoadIcon(NULL, IDI_APPLICATION), 
        LoadCursor(NULL, IDC_ARROW), 
        GetStockObject(BLACK_BRUSH),
        NULL, "WindowClass"
    };

    Renderer renderer;
    int w = (int)(app->window_w * app->render_scale);
    int h = (int)(app->window_h * app->render_scale);
    resize_dib_section(&app->frame_buffer, w, h);
    gfx_api->init(w, h);
    renderer_init(&renderer, w, h);

    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT rc;
    int screen_w = GetSystemMetrics(SM_CXSCREEN);
    int screen_h = GetSystemMetrics(SM_CYSCREEN);
    rc.left = (screen_w - app->window_w) / 2;
    rc.top  = (screen_h - app->window_h) / 2;
    rc.right = rc.left + app->window_w;
    rc.bottom = rc.top + app->window_h;
    AdjustWindowRect(&rc, style, FALSE);

    ATOM w_class = RegisterClassA(&wc);
    HWND hwnd = CreateWindowA(
        MAKEINTATOM(w_class),
        "Software Rasterizer",
        style,
        rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    ShowWindow(hwnd, SW_SHOW);
    HDC dc = GetDC(hwnd);
    app->running = true;
    float t = 0.0f;

    uint64_t global_clock = os_time_now();

    while (app->running) {
        uint64_t frame_start = os_time_now();
        float dt = (float)os_time_delta(frame_start, global_clock);
        global_clock = frame_start;

        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                app->running = false;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        renderer_draw_scene(&renderer, t);
        t += dt;

        Frame_Buffer *buffer = &app->frame_buffer;
        gfx_api->swap_buffers(buffer->data);
        StretchDIBits(dc, 
                0, 0, app->window_w, app->window_h,
                0, 0, buffer->width, buffer->height,
                buffer->data,
                &buffer->info,
                DIB_RGB_COLORS, SRCCOPY);
    }

    gfx_api->shutdown();

    extern int64_t allocated_bytes;
    printf("Leaked bytes: %zi (%.2fKB)\n", allocated_bytes, allocated_bytes / 1000.f);

    ReleaseDC(hwnd, dc);
    return 0;
}
