#include "fsiege.h"

#include "edge.h"

#include <math.h>

/**
 * C - circle
 */
void _sgIntersectCL(SGVec2 c, float r, SGVec2 p1, SGVec2 p2, SGVec2* i1, SGVec2* i2, float* discr)
{
    p1 = sgVec2Sub(p1, c);
    p2 = sgVec2Sub(p2, c);

    SGVec2 d = sgVec2Sub(p2, p1);
    float dr = sgVec2Length(d);
    float dr2 = dr*dr;

    float D = p1.x * p2.y - p2.x * p1.y;

    *discr = r*r * dr2 - D*D;

    i1->x = ( D * d.y + SG_PSIGN(d.y) * d.x * sqrt(*discr)) / dr2;
    i1->y = (-D * d.x +         SG_ABS(d.y) * sqrt(*discr)) / dr2;
    *i1 = sgVec2Add(*i1, c);

    i2->x = ( D * d.y - SG_PSIGN(d.y) * d.x * sqrt(*discr)) / dr2;
    i2->y = (-D * d.x -         SG_ABS(d.y) * sqrt(*discr)) / dr2;
    *i2 = sgVec2Add(*i2, c);
}
size_t sgIntersectCL(SGVec2 c, float r, SGVec2 p1, SGVec2 p2, SGVec2* i1, SGVec2* i2)
{
    SGVec2 tmp;
    if(!i1) i1 = &tmp;
    if(!i2) i2 = &tmp;

    float discr;
    _sgIntersectCL(c, r, p1, p2, i1, i2, &discr);

    if(discr > 0)
        return 2;
    if(discr == 0)
        return 1;
    return 0;
}
size_t sgIntersectCS(SGVec2 c, float r, SGVec2 p1, SGVec2 p2, SGVec2* i1, SGVec2* i2)
{
    SGVec2 tmp1;
    SGVec2 tmp2;
    if(!i1) i1 = &tmp1;
    if(!i2) i2 = &tmp2;

    size_t num = sgIntersectCL(c, r, p1, p2, i1, i2);

    SGbool ir1 = SG_IN_XRANGE(i1->x, p1.x, p2.x) && SG_IN_XRANGE(i1->y, p1.y, p2.y);
    SGbool ir2 = SG_IN_XRANGE(i2->x, p1.x, p2.x) && SG_IN_XRANGE(i2->y, p1.y, p2.y);

    if(num == 2)
    {
        if(ir1 && ir2)
            return 2;
        else if(ir1)
            return 1;
        else if(ir2)
        {
            SGVec2 tmp = *i1; // we swap them so that i1 is the one in range
            *i1 = *i2;
            *i2 = tmp;
            return 1;
        }
        return 0;
    }
    if(num == 1)
    {
        if(ir1 && ir2) // we actually return 2 here
            return 2;
        return 0;
    }
    return 0;
}

SGVec2 getMousePos(void)
{
    SGint mx, my;
    sgMouseGetPos(&mx, &my);
    SGVec2 mpos = sgVec2f(mx, my);

    mpos.x = viewport->posx + mpos.x * viewport->sizex / viewport->wsizex;
    mpos.y = viewport->posy + mpos.y * viewport->sizey / viewport->wsizey;

    return mpos;
}

float lerp(float t, float a, float b)
{
    return a + t * (b - a);
}
SGVec2 lerp2(float t, SGVec2 a, SGVec2 b)
{
    SGVec2 v;
    v.x = lerp(t, a.x, b.x);
    v.y = lerp(t, a.y, b.y);
    return v;
}

float frand2(float min, float max)
{
    return min + rand() / (float)RAND_MAX * (max - min);
}

void drawEArcRads(float x, float y, float rx, float ry, float a1, float a2, SGbool ccw, SGbool fill, SGColor a, SGColor b)
{
    float adiff = a2 - a1;

    if(!ccw && adiff < 0)
        adiff = adiff + 2 * SG_PI;
    else if(ccw && adiff > 0)
        adiff = adiff - 2 * SG_PI;

    float ra = (rx + ry) / 2.0;
    SGuint numsides = (SGuint)(ra * SG_ABS(adiff) / 2 / SG_PI * 4);
    adiff /= numsides;
    SGuint i;

    if(fill)
    {
        sgDrawBegin(SG_TRIANGLE_FAN);
        sgDrawColor4fv(&a.r);
        sgDrawVertex2f(x, y);
    }
    else
        sgDrawBegin(SG_LINE_STRIP);

    sgDrawColor4fv(&b.r);
    for(i = 0; i <= numsides; i++)
    {
        float a = a1 + i * adiff;
        sgDrawVertex2f(x + cos(a) * rx, y + sin(a) * ry);
    }

    sgDrawEnd();
}
void drawEArcRads2(float x, float y, float rx1, float ry1, float rx2, float ry2, float a1, float a2, SGbool ccw, SGbool fill, SGColor ac, SGColor bc)
{
    float adiff = a2 - a1;

    if(!ccw && adiff < 0)
        adiff = adiff + 2 * SG_PI;
    else if(ccw && adiff > 0)
        adiff = adiff - 2 * SG_PI;

    float ra = (rx1 + ry1 + rx2 + ry2) / 4.0;
    SGuint numsides = (SGuint)(ra * SG_ABS(adiff) / 2 / SG_PI * 4);
    adiff /= numsides;
    SGuint i;

    if(fill)
    {
        sgDrawBegin(SG_TRIANGLE_STRIP);
    }
    else
        sgDrawBegin(SG_LINE_STRIP);

    sgDrawColor4fv(&bc.r);
    for(i = 0; i <= numsides; i++)
    {
        float a = a1 + i * adiff;
        if(fill)
        {
            sgDrawColor4fv(&ac.r);
            sgDrawVertex2f(x + cos(a) * rx1, y + sin(a) * ry1);
            sgDrawColor4fv(&bc.r);
        }

        sgDrawVertex2f(x + cos(a) * rx2, y + sin(a) * ry2);
    }

    sgDrawEnd();

    if(!fill)
    {
        sgDrawBegin(SG_LINE_STRIP);
        sgDrawColor4fv(&ac.r);
        for(i = 0; i <= numsides; i++)
        {
            float a = a1 + i * adiff;
            sgDrawVertex2f(x + cos(a) * rx1, y + sin(a) * ry1);
        }
        sgDrawEnd();
    }
}

SGVec2 nearestInter(SGVec2 pos, SGVec2 ray, struct Edge* ignore, struct Edge** iedge)
{
    float ilen;
    SGVec2 inter;
    *iedge = NULL;
    SGbool hasinter;

    float clen;
    SGVec2 curr;
    SGbool refl = SG_FALSE;

    SGListNode* node;
    Edge* edge;
    for(node = edges->head; node; node = node->next)
    {
        edge = node->item;
        if(edge == ignore)
            continue;

        hasinter = sgIntersectSR(&curr, edge->head, edge->tail, pos, sgVec2Add(pos, ray));
        if(hasinter)
        {
            clen = sgVec2Length(sgVec2Sub(curr, pos));
            if(!refl || clen < ilen)
            {
                ilen = clen;
                inter = curr;
                *iedge = edge;
            }
            refl = SG_TRUE;
        }
    }
    return inter;
}

#define NREFLECTS 5

void reflectRay(SGVec2 pos, SGVec2 ray, SGColor color, size_t num, struct Edge* ignore)
{
    if(num > NREFLECTS)
        return;

    float ilen;
    SGVec2 inter;
    Edge* iedge;
    SGVec2 reflect;
    SGbool hasinter;

    float clen;
    SGVec2 curr;
    SGbool refl = SG_FALSE;

    SGListNode* node;
    Edge* edge;
    for(node = edges->head; node; node = node->next)
    {
        edge = node->item;
        if(edge == ignore)
            continue;

        hasinter = sgIntersectSR(&curr, edge->head, edge->tail, pos, sgVec2Add(pos, ray));
        if(hasinter)
        {
            clen = sgVec2Length(sgVec2Sub(curr, pos));
            if(!refl || clen < ilen)
            {
                ilen = clen;
                inter = curr;
                iedge = edge;
            }
            refl = SG_TRUE;
        }
    }
    if(refl)
    {
        sgDrawBegin(SG_LINES);
        sgDrawColor4f(color.r, color.g, color.b, color.a * (1.0 - num / (float)NREFLECTS));
        sgDrawVertex2f(pos.x, pos.y);
        sgDrawColor4f(color.r, color.g, color.b, color.a * (1.0 - (num + 1.0) / (float)NREFLECTS));
        sgDrawVertex2f(inter.x, inter.y);
        sgDrawEnd();
        reflect = sgVec2Reflect(ray, edgeNormal(iedge));
        reflectRay(inter, reflect, color, num + 1, iedge);
    }
}
