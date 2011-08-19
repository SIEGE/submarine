#include "pedge.h"


void lcPingEdgeDestroy(SGEntity* entity)
{
    free(entity->data);
}
void evPingEdgeTick(SGEntity* entity)
{
    PingEdge* pe = entity->data;

    if(pe->str <= 0.0)
    {
        sgEntityDestroy(entity);
        return;
    }

    pe->str -= pe->dim;//1.0/512.0;
}
void evPingEdgeDraw(SGEntity* entity)
{
    PingEdge* pe = entity->data;

    float threshold = 0.95;
    float factor = (pe->str - threshold) * 1.0 / (1.0 - threshold);

    SGColor c;
    if(pe->str > threshold)
        c = sgColor4f(factor, 0.75 + 0.25 * factor, 1.0, pe->str);
    else
        c = sgColor4f(0.0, 0.75, 1.0, pe->str);

    sgDrawBegin(SG_GRAPHICS_PRIMITIVE_LINES);
            sgDrawColor4f(c.r, c.g, c.b, pe->halpha * c.a);
            sgDrawVertex2fv(&pe->edge.head.x);
            sgDrawColor4f(c.r, c.g, c.b, pe->talpha * c.a);
            sgDrawVertex2fv(&pe->edge.tail.x);
    sgDrawEnd();
}

PingEdge* pingEdgeCreate(SGVec2 head, SGVec2 tail, float dim)
{
    PingEdge* pe = malloc(sizeof(PingEdge));
    if(!pe)
        return NULL;

    pe->entity = sgEntityCreate(0.0);
    pe->entity->data = pe;
    pe->entity->lcDestroy = lcPingEdgeDestroy;
    pe->entity->evTick = evPingEdgeTick;
    pe->entity->evDraw = evPingEdgeDraw;

    pe->edge.head = head;
    pe->edge.tail = tail;
    pe->halpha = 1.0;
    pe->talpha = 1.0;
    pe->str = 1.0;
    pe->dim = dim;

    return pe;
}
