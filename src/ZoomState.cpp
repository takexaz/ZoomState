#include <StateControllerExtension.h>
#include "_ZoomState.h"

int gx = 0;
int gy = 0;
float gscale = 1.0;

// FALSE=Error
int zoomReg(TPFILE* tpf, STATE_INFO* sinfo, PLAYER_CACHE* pcache) {
    ST_ZOOM* zoom = new ST_ZOOM;
    sinfo->params = zoom;
    DWORD TEMP;

    const char* value;
    int num;
    value = TPGetValue(tpf, "pos");
    if (value) {
        num = SCtrlReadExpList(value, "ii", pcache, &TEMP, &zoom->x, &zoom->y);
        if (num != 2) {
            setErrorText("Illegal pos parameter for zoom");
            return FALSE;
        }
    }
    else {
        setErrorText("Missing pos parameter for zoom");
        return FALSE;
    }

    value = TPGetValue(tpf, "scale");
    if (value) {
        num = SCtrlReadExpList(value, "f", pcache, &TEMP, &zoom->scale);
        if (num != 1) {
            setErrorText("Illegal scale parameter for zoom");
            return FALSE;
        }
    }
    else {
        setErrorText("Missing scale parameter for zoom");
        return FALSE;
    }

    return TRUE;
}

void zoomProc(PLAYER* p, STATE_INFO* sinfo) {
    ST_ZOOM* zoom = (ST_ZOOM*)sinfo->params;
    gx = EvalExpression(p, &zoom->x, 0);
    if (gx < 0) {
        VWarn(p, "zoom pos x too small: %d", gx);
    }
    else if (gx > 320) {
        VWarn(p, "zoom pos x too big: %d", gx);
    }
    gy = EvalExpression(p, &zoom->y, 0);
    if (gy < 0) {
        VWarn(p, "zoom pos y too small: %d", gx);
    }
    else if (gy > 240) {
        VWarn(p, "zoom pos y too big: %d", gx);
    }
    gscale = EvalExpression(p, &zoom->scale);
    if (gscale < 1.0) {
        VWarn(p, "zoom scale too small: %f", gscale);
    }
    return;
}

void zoomFree(STATE_INFO* sinfo) {
    ST_ZOOM* zoom = (ST_ZOOM*)sinfo->params;
    FreeExpression(&zoom->x);
    FreeExpression(&zoom->y);
    FreeExpression(&zoom->scale);
    delete zoom;
    return;
}

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