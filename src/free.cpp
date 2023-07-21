#include <StateControllerExtension.h>
#include "_ZoomState.h"

void stxFree(STATE_INFO* sinfo) {
    ST_ZOOM* zoom = (ST_ZOOM*)sinfo->params;
    FreeExpression(&zoom->x);
    FreeExpression(&zoom->y);
    FreeExpression(&zoom->scale);
    delete zoom;
    return;
}