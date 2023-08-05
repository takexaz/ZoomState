#include <StateControllerExtension.h>
#include <Winmugen.h>
#include "_ZoomState.h"

int stxRegister(TPFILE* tpf, MUGEN_SC_DATA_EX* scdx, MUGEN_PLAYER_INFO* pinfo) {
    ST_ZOOM* zoom = new ST_ZOOM;
    scdx->SCX->params = zoom;
    const char* parseEnd;

    const char* value;
    int num;
    value = TPGetValue(tpf, "pos");
    if (value) {
        num = SCtrlReadExpList(value, "ii", pinfo, &parseEnd, &zoom->x, &zoom->y);
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
        num = SCtrlReadExpList(value, "f", pinfo, &parseEnd, &zoom->scale);
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