#include <StateControllerExtension.h>
#include "_ZoomState.h"

void zoomScreen(ALLEG_BITMAP* screen, int x, int y, float scale) {

    // スクリーンの幅と高さを取得
    int SCREEN_W = screen->w;
    int SCREEN_H = screen->h;

    // スクリーンをバックアップ
    ALLEG_BITMAP* screenBackup = create_bitmap(SCREEN_W, SCREEN_H);
    blit(screen, screenBackup, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    // 320x240の範囲に収める
    x = clamp(x, 0, 320);
    y = clamp(y, 0, 240);

    float perW = SCREEN_W / scale;
    float perH = SCREEN_H / scale;

    int x1 = floor(x * (SCREEN_W - perW) / 320.0f);
    int x2 = floor(perW);
    int y1 = floor(y * (SCREEN_H - perH) / 240.0f);
    int y2 = floor(perH);

    // バックアップしたスクリーンを拡大して描画
    stretch_blit(screenBackup, screen, x1, y1, x2, y2, 0, 0, SCREEN_W, SCREEN_H);

    // メモリの解放
    destroy_bitmap(screenBackup);
}

void zoomHook(void) {
    if (gscale > 1.0) {
        // Screen取得
        DWORD V_Current = *((DWORD*)0x4B6038);
        DWORD V_CurrentIdx = *((DWORD*)0x4B603C);
        DWORD* sprArray = SpriteArrayAccess(V_Current, V_CurrentIdx);
        ALLEG_BITMAP* screen = (ALLEG_BITMAP*)*(sprArray + 9);
        zoomScreen(screen, gx, gy, gscale);
        gx = 0;
        gy = 0;
        gscale = 1.0;
    }

    return;
}