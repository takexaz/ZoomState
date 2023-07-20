﻿// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"
#include <Mebius.h>
#include <StateControllerExtension.h>
#pragma comment(lib, "Mebius.lib")
#pragma comment(lib, "StateControllerExtension.lib")

auto SpriteArrayAccess = reinterpret_cast<DWORD * (*)(DWORD ptr, DWORD ptr2)>(0x44eeb0);
auto DrawToScreen = reinterpret_cast<void (*)(void)>(0x4174e0);

struct ALLEG_BITMAP {
    int w, h;
    int clip;
    int cl, cr, ct, cb;
    unsigned char* line;
};
HINSTANCE allegro = GetModuleHandleA((LPCSTR)"alleg40");
auto create_bitmap = reinterpret_cast<ALLEG_BITMAP * (*)(int width, int height)>(GetProcAddress(allegro, "create_bitmap"));
auto destroy_bitmap = reinterpret_cast<void (*)(ALLEG_BITMAP * bitmap)>(GetProcAddress(allegro, "destroy_bitmap"));
auto stretch_blit = reinterpret_cast<void (*)(ALLEG_BITMAP * source, ALLEG_BITMAP * dest, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y, int dest_w, int dest_h)>(GetProcAddress(allegro, "stretch_blit"));
auto blit = reinterpret_cast<void (*)(ALLEG_BITMAP * source, ALLEG_BITMAP * dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height)>(GetProcAddress(allegro, "blit"));

int zoomReg(TPFILE*, STATE_INFO*, PLAYER_CACHE*);
void zoomProc(PLAYER*, STATE_INFO*);
void zoomFree(STATE_INFO*);
void zoomScreen(ALLEG_BITMAP*, int, int, float);
void zoomHook(void);

struct ST_ZOOM {
    EVAL_EXP x,y;
    EVAL_EXP scale;
};

int x = 0;
int y = 0;
float scale = 1.0;

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
            setErrorText("Illegal pos parameter");
            return FALSE;
        }
    }
    else {
        setErrorText("Missing pos parameter");
        return FALSE;
    }

    value = TPGetValue(tpf, "scale");
    if (value) {
        num = SCtrlReadExpList(value, "f", pcache, &TEMP, &zoom->scale);
        if (num != 1) {
            setErrorText("Illegal scale parameter");
            return FALSE;
        }
    }
    else {
        setErrorText("Missing scale parameter");
        return FALSE;
    }

    return TRUE;
}

void zoomProc(PLAYER* p, STATE_INFO* sinfo) {
    ST_ZOOM* zoom = (ST_ZOOM*)sinfo->params;
    x = EvalExpression(p,&zoom->x, 0);
    y = EvalExpression(p, &zoom->y, 0);
    scale = EvalExpression(p, &zoom->scale);
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

void zoomScreen(ALLEG_BITMAP* screen, int posx, int posy, float scale) {

    // スクリーンの幅と高さを取得
    int SCREEN_W = screen->w;
    int SCREEN_H = screen->h;

    // スクリーンをバックアップ
    ALLEG_BITMAP* screenBackup = create_bitmap(SCREEN_W, SCREEN_H);
    blit(screen, screenBackup, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    // 320x240の範囲に収める
    posx = min(max(0, posx), 320);
    posy = min(max(0, posy), 240);

    float perW = SCREEN_W / scale;
    float perH = SCREEN_H / scale;

    int x1 = floor(posx * (SCREEN_W - perW) / 320.0f);
    int x2 = floor(perW);
    int y1 = floor(posy * (SCREEN_H - perH) / 240.0f);
    int y2 = floor(perH);

    // バックアップしたスクリーンを拡大して描画
    stretch_blit(screenBackup, screen, x1, y1, x2, y2, 0, 0, SCREEN_W, SCREEN_H);

    // メモリの解放
    destroy_bitmap(screenBackup);
}

void zoomHook(void) {
    if (scale > 1.0) {
        // Screen取得
        DWORD V_Current = *((DWORD*)0x4B6038);
        DWORD V_CurrentIdx = *((DWORD*)0x4B603C);
        DWORD* sprArray = SpriteArrayAccess(V_Current, V_CurrentIdx);
        ALLEG_BITMAP* screen = (ALLEG_BITMAP*)*(sprArray + 9);
        zoomScreen(screen, x, y, scale);
        x = 0;
        y = 0;
        scale = 1.0;
    }

    return;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        Hook(DrawToScreen, zoomHook, HEAD);
        STX zoom = {
            "zoom",
            zoomReg,
            zoomProc,
            zoomFree,
        };
        addState(zoom);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

