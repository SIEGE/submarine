#ifndef __PEDGE_H__
#define __PEDGE_H__

#include "common.h"
#include "edge.h"

typedef struct PingEdge
{
    SGEntity* entity;
    Edge edge;
    float halpha;
    float talpha;
    float str;
    float dim;
} PingEdge;

void lcPingEdgeDestroy(SGEntity* entity);
void evPingEdgeTick(SGEntity* entity);
void evPingEdgeDraw(SGEntity* entity);

PingEdge* pingEdgeCreate(SGVec2 head, SGVec2 tail, float dim);

#endif // __PEDGE_H__
