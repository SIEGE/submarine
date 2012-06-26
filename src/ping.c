#include "ping.h"

#include "fsiege.h"

void lcPingDestroy(SGEntity* entity)
{
    free(entity->data);
}
void evPingTick(SGEntity* entity)
{
    Ping* ping = entity->data;

    if(ping->str <= 0.0)
    {
        sgEntityDestroy(entity);
        return;
    }

    Edge* edge;
    SGVec2 inter = nearestInter(ping->pos, ping->vel, ping->edge, &edge);

    float dist = sgVec2GetLength(sgVec2Sub(inter, ping->pos));

    if(edge && dist < sgVec2GetLength(ping->vel))
    {
        SGAudioSource* srcPing = sgAudioSourceCreate(1.0, 0.25, 2.0, SG_FALSE);
        sgAudioSourceQueueBuffer(srcPing, bufPing);
        sgAudioSourcePlay(srcPing);
        sgAudioSourceDestroyLazy(srcPing);

        ping->pos = inter;
        ping->vel = sgVec2SetLength(sgVec2Reflect(ping->vel, edgeNormal(edge)), sgVec2GetLength(ping->vel));
        ping->edge = edge;

        lightEdges(ping->pos, ping->radius, 1.0/512.0);
    }
    else
        ping->pos = sgVec2Add(ping->pos, ping->vel);
    //ping->str -= 1.0/1024.0;
    ping->str -= 1.0/256.0;
}
void evPingDraw(SGEntity* entity)
{
    /*Ping* ping = entity->data;

    sgDrawColor4f(0.0, 0.0, 1.0, ping->str);
    sgDrawCircle(ping->pos.x, ping->pos.y, 8, SG_TRUE);*/
}

Ping* SG_EXPORT pingCreate(SGVec2 pos, SGVec2 vel, float str, float radius, Edge* edge)
{
    Ping* ping = malloc(sizeof(Ping));
    if(!ping)
        return NULL;

    ping->entity = sgEntityCreate();
    ping->entity->data = ping;
    ping->entity->lcDestroy = lcPingDestroy;
    ping->entity->evTick = evPingTick;
    ping->entity->evDraw = evPingDraw;

    ping->pos = pos;
    ping->vel = vel;
    ping->str = str;
    ping->radius = radius;
    ping->edge = edge;

    return ping;
}
