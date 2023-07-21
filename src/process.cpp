#include <StateControllerExtension.h>
#include "_ZoomState.h"

void stxProcess(PLAYER* p, STATE_INFO* sinfo) {
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