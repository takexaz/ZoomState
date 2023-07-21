#include <Mebius.h>
#include <StateControllerExtension.h>
#include "_ZoomState.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        Hook(DrawToScreen, zoomHook, HEAD);
        STX zoom = {
            "zoom",
            stxRegister,
            stxProcess,
            stxFree,
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
