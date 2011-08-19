#include "edge.h"

#include "fsiege.h"
#include "pedge.h"

SGVec2 edgeCenter(Edge* edge)
{
    SGVec2 v = sgVec2Add(edge->head, edge->tail);
    v.x *= 0.5;
    v.y *= 0.5;
    return v;
}
SGVec2 edgeNormal(Edge* edge)
{
    SGVec2 v = sgVec2Sub(edge->tail, edge->head);
    return sgVec2Normalize(sgVec2f(v.y, -v.x));
}

void destroyEdge(Edge* edge)
{
    if(!edge)
        return;
    sgListRemoveNode(edge->list, edge->node);
    free(edge);
}
Edge* createEdge(SGList* list, SGVec2 head, SGVec2 tail)
{
    Edge* edge = malloc(sizeof(Edge));
    if(!edge)
        return NULL;
    edge->head = head;
    edge->tail = tail;

    edge->list = list;
    edge->node = sgListAppend(list, edge);

    return edge;
}

void drawEdgeDBG(Edge* edge)
{
    sgDrawColor4f(1.0, 1.0, 1.0, 0.0625);
    sgDrawLine(edge->head.x, edge->head.y, edge->tail.x, edge->tail.y);
    sgDrawColor4f(1.0, 0.0, 0.0, 1.0);
    SGVec2 c = edgeCenter(edge);
    SGVec2 n = edgeNormal(edge);
    sgDrawLine(c.x, c.y, c.x + n.x * 16, c.y + n.y * 16);
    sgDrawColor4f(1.0, 1.0, 1.0, 1.0);
}

void lightEdges(SGVec2 pos, float radius, float dim)
{
    SGVec2 i1;
    SGVec2 i2;
    SGVec2 proj;
    size_t numi;

    float pdist;
    float phdist;
    float ptdist;
    float hdist;
    float tdist;
    float idist;

    SGListNode* node;
    Edge* edge;
    PingEdge* nedge;
    for(node = edges->first; node; node = node->next)
    {
        edge = node->item;

        numi = sgIntersectCS(pos, radius, edge->head, edge->tail, &i1, &i2);
        if(numi == 2)
        {
            proj = sgVec2Project(sgVec2Sub(pos, edge->head), sgVec2Sub(edge->tail, edge->head));
            proj = sgVec2Add(proj, edge->head);
            pdist = distance(proj, pos);

            nedge = pingEdgeCreate(i1, proj, dim);
            nedge->halpha = 0.0;
            nedge->talpha = 1.0 - pdist / radius;
            nedge = pingEdgeCreate(proj, i2, dim);
            nedge->halpha = 1.0 - pdist / radius;
            nedge->talpha = 0.0;
        }
        else if(numi == 1)
        {
            hdist = distance(pos, edge->head);
            tdist = distance(pos, edge->tail);

            proj = sgVec2Project(sgVec2Sub(pos, edge->head), sgVec2Sub(edge->tail, edge->head));
            proj = sgVec2Add(proj, edge->head);
            if((edge->head.x == edge->tail.x || SG_IN_RANGE(proj.x, edge->head.x, edge->tail.x))
            && (edge->head.y == edge->tail.y || SG_IN_RANGE(proj.y, edge->head.y, edge->tail.y)))
                pdist = distance(proj, pos);
            else
            {
                phdist = distance(pos, edge->head);
                ptdist = distance(pos, edge->tail);
                if(phdist < ptdist)
                {
                    proj = edge->head;
                    pdist = phdist;
                }
                else
                {
                    proj = edge->tail;
                    pdist = ptdist;
                }
            }

            idist = distance(pos, i1);

            if(hdist <= radius)
            {
                nedge = pingEdgeCreate(edge->head, proj, dim);
                nedge->halpha = 1.0 - hdist / radius;
                nedge->talpha = 1.0 - pdist / radius;
                nedge = pingEdgeCreate(proj, i1, dim);
                nedge->halpha = 1.0 - pdist / radius;
                nedge->talpha = 1.0 - idist / radius;
            }
            else // distance(pos, edge->tail) <= radius
            {
                nedge = pingEdgeCreate(i1, proj, dim);
                nedge->halpha = 1.0 - idist / radius;
                nedge->talpha = 1.0 - pdist / radius;
                nedge = pingEdgeCreate(proj, edge->tail, dim);
                nedge->halpha = 1.0 - pdist / radius;
                nedge->talpha = 1.0 - tdist / radius;
            }
        }
        else if(numi == 0)
        {
            hdist = distance(pos, edge->head);
            tdist = distance(pos, edge->tail);
            if(hdist <= radius) // tdist is as well then!
            {
                proj = sgVec2Project(sgVec2Sub(pos, edge->head), sgVec2Sub(edge->tail, edge->head));
                proj = sgVec2Add(proj, edge->head);
                if((edge->head.x == edge->tail.x || SG_IN_RANGE(proj.x, edge->head.x, edge->tail.x))
                && (edge->head.y == edge->tail.y || SG_IN_RANGE(proj.y, edge->head.y, edge->tail.y)))
                    pdist = distance(proj, pos);
                else
                {
                    phdist = distance(pos, edge->head);
                    ptdist = distance(pos, edge->tail);
                    if(phdist < ptdist)
                    {
                        proj = edge->head;
                        pdist = phdist;
                    }
                    else
                    {
                        proj = edge->tail;
                        pdist = ptdist;
                    }
                }

                phdist = distance(pos, edge->head);
                ptdist = distance(pos, edge->tail);

                nedge = pingEdgeCreate(edge->head, proj, dim);
                nedge->halpha = 1.0 - phdist / radius;
                nedge->talpha = 1.0 - pdist / radius;
                nedge = pingEdgeCreate(proj, edge->tail, dim);
                nedge->halpha = 1.0 - pdist / radius;
                nedge->talpha = 1.0 - ptdist / radius;
            }
        }
    }
}
