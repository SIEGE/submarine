#ifndef __EDGE_H__
#define __EDGE_H__

#include "common.h"

typedef struct Edge
{
    SGList* list;
    SGListNode* node;
    SGVec2 head;
    SGVec2 tail;
} Edge;
SGList* edges;

SGVec2 edgeCenter(Edge* edge);
SGVec2 edgeNormal(Edge* edge);

void destroyEdge(Edge* edge);
Edge* createEdge(SGList* list, SGVec2 head, SGVec2 tail);

void drawEdgeDBG(Edge* edge);

void lightEdges(SGVec2 pos, float radius, float dim);

#endif // __EDGE_H__
