#include <StateControllerExtension.h>
#include "_ZoomState.h"

void stxFree(MUGEN_SC_DATA_EX* scdx) {
    ST_ZOOM* zoom = (ST_ZOOM*)scdx->SCX->params;
    FreeExpression(&zoom->x);
    FreeExpression(&zoom->y);
    FreeExpression(&zoom->scale);
    delete zoom;
    return;
}