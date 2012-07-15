/*
    This will eventually be thrown into SIEGE in one form or another.
*/

#ifndef __FSIEGE_H__
#define __FSIEGE_H__

#include "common.h"

//#define SG_IN_RANGE(x, min, max) (((min) <= (x)) && ((x) <= (max)))
#define SG_IN_RANGE(x, min, max) ((SG_MIN(min, max) <= (x)) && ((x) <= SG_MAX(min, max)))
#define SG_IN_CRANGE(x, min, max) (((min) == (max)) || SG_IN_RANGE(x, min, max))
#define SG_SIGN(x) (((x) > 0) - ((x) < 0))
#define SG_NSIGN(x) (((x) < 0) ? -1 : 1)

SGVec2 _sgIntersectLL(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, float* den, SGVec2* u);
SGVec2 sgIntersectLL(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, SGbool* inter);

SGVec2 sgIntersectSS(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, SGbool* inter);
SGVec2 sgIntersectLS(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, SGbool* inter);
SGVec2 sgIntersectSL(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, SGbool* inter);
SGVec2 sgIntersectRR(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, SGbool* inter);
SGVec2 sgIntersectLR(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, SGbool* inter);
SGVec2 sgIntersectRL(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, SGbool* inter);
SGVec2 sgIntersectSR(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, SGbool* inter);
SGVec2 sgIntersectRS(SGVec2 p1, SGVec2 p2, SGVec2 p3, SGVec2 p4, SGbool* inter);

/**
 * C - circle
 */
void _sgIntersectCL(SGVec2 c, float r, SGVec2 p1, SGVec2 p2, SGVec2* i1, SGVec2* i2, float* discr);
size_t sgIntersectCL(SGVec2 c, float r, SGVec2 p1, SGVec2 p2, SGVec2* i1, SGVec2* i2);
size_t sgIntersectCS(SGVec2 c, float r, SGVec2 p1, SGVec2 p2, SGVec2* i1, SGVec2* i2);

// will eventually become a viewport function to transform coords into viewport coords
SGVec2 getMousePos(void);

// will take and return a float in interpolate.c
float lerp(float t, float a, float b);
SGVec2 lerp2(float t, SGVec2 a, SGVec2 b);

// will be used in rand.c
float frand2(float min, float max);

// will use a callback or similar for drawing
void drawEArcRads(float x, float y, float rx, float ry, float a1, float a2, SGbool ccw, SGbool fill, SGColor a, SGColor b);
void drawEArcRads2(float x, float y, float rx1, float ry1, float rx2, float ry2, float a1, float a2, SGbool ccw, SGbool fill, SGColor a, SGColor b);

struct Edge;

// will also be in SIEGE in one form or another
SGVec2 nearestInter(SGVec2 pos, SGVec2 ray, struct Edge* ignore, struct Edge** iedge);

void reflectRay(SGVec2 pos, SGVec2 ray, SGColor color, size_t num, struct Edge* ignore);

#endif // __FSIEGE_H__
