#ifndef __PING_H__
#define __PING_H__

#include "common.h"
#include "edge.h"

typedef struct Ping
{
    SGEntity* entity;
    SGVec2 pos;
    SGVec2 vel;
    float str;
    //float strdim;
    float radius;
    Edge* edge;
} Ping;

void lcPingDestroy(SGEntity* entity);
void evPingTick(SGEntity* entity);
void evPingDraw(SGEntity* entity);

Ping* SG_EXPORT pingCreate(SGVec2 pos, SGVec2 vel, float str, float radius, Edge* edge);

#endif // __PING_H__
