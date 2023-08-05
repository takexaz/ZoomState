#include <StateControllerExtension.h>
#include <Winmugen.h>
#include "_ZoomState.h"

void stxProcess(MUGEN_PLAYER* player, MUGEN_SC_DATA_EX* scdx) {
    ST_ZOOM* zoom = (ST_ZOOM*)scdx->SCX->params;
    gx = EvalExpressionN(player, &zoom->x, 0);
    if (gx < 0) {
        VWarn(player, "zoom pos x too small: %d", gx);
    }
    else if (gx > 320) {
        VWarn(player, "zoom pos x too big: %d", gx);
    }
    gy = EvalExpressionN(player, &zoom->y, 0);
    if (gy < 0) {
        VWarn(player, "zoom pos y too small: %d", gx);
    }
    else if (gy > 240) {
        VWarn(player, "zoom pos y too big: %d", gx);
    }
    gscale = EvalExpressionN(player, &zoom->scale);
    if (gscale < 1.0) {
        VWarn(player, "zoom scale too small: %f", gscale);
    }
    return;
}