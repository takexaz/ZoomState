#include <StateControllerExtension.h>
#include "_ZoomState.h"

int stxRegister(TPFILE* tpf, STATE_INFO* sinfo, PLAYER_CACHE* pcache) {
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