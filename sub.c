#include "sub.h"

#include "fsiege.h"
#include "edge.h"
#include "ping.h"

#include <math.h>

void drawBand(float x, float y, float r1, float r2, SGColor a, SGColor b)
{
    /*x += 32;
    y -= 32;
    r1 *= 2;
    r2 *= 2;*/

    drawEArcRads2(x, y, r1, r1, r2, r2, 0, 2 * SG_PI, SG_FALSE, SG_TRUE, a, b);
    //drawEArcRads2(x, y, r1, r1, r2, r2, 0, 2 * SG_PI, SG_FALSE, SG_FALSE, a, b);
}
void drawBands(float x, float y, float* bands, size_t numbands, SGColor a, SGColor b)
{
    float maxband = bands[numbands - 1];
    size_t i;
    for(i = 0; i < numbands; i += 2)
        drawBand(x, y, bands[i], bands[i+1], sgColor4f(a.r, a.g, a.b, a.a * (1.0 - bands[i] / maxband)), sgColor4f(b.r, b.g, b.b, b.a * (1.0 - bands[i+1] / maxband)));
}
void drawRects(float x, float y, size_t mask, SGColor c)
{
    sgDrawColor4f(c.r, c.g, c.b, c.a * 29.0/255.0);
    if(mask >= 3)
        sgDrawColor4f(c.r, c.g, c.b, c.a);
    sgDrawRectangleWH(x - 16 + 2, y + 16 - 8, 5, 1, SG_TRUE);
    if(mask >= 2)
        sgDrawColor4f(c.r, c.g, c.b, c.a);
    sgDrawRectangleWH(x - 16 + 2, y + 16 - 6, 5, 1, SG_TRUE);
    if(mask >= 1)
        sgDrawColor4f(c.r, c.g, c.b, c.a);
    sgDrawRectangleWH(x - 16 + 2, y + 16 - 4, 5, 1, SG_TRUE);
    if(mask >= 0) // always
        sgDrawColor4f(c.r, c.g, c.b, c.a);
    sgDrawRectangleWH(x - 16 + 2, y + 16 - 2, 5, 1, SG_TRUE);
}

void subDrawMode(SGEntity* entity)
{
    Sub* sub = entity->data;

    SGVec2 pos;
    sgEntityGetPos(entity, &pos.x, &pos.y);

    sgDrawColor4f(0.0, 0.5, 0.75, 1.0);
    sgViewportSet4i(viewport, 0, 0, 640, 480);

    SGColor umask = sgColor4f(0.0, 0.25, 0.5, 1.0); // unselected
    SGColor smask = sgColor4f(0.0, 0.5, 0.75, 1.0); // selected
    SGColor color;

    float x, y;
    float c1;
    float c2;
    y = 480 - 16.0;

    // 6.20325
    // 9.304875
    float bands2[] = { 5.375, 6.625, 7.5, 8.5, 9.563, 10.438, 11.594, 12.4065 };
    float bands3[] = { 5.375, 6.625, 7.5, 8.5, 9.5, 10.5, 11.531, 12.469, 13.594, 14.4065 };

    color = (sub->mode == 0) ? smask : umask;
    x = 0 * 32 + 16.0;
    drawBand(x, y, 0, 4, sgColor4f(color.r, color.g, color.b, color.a * 0.25/*34.0/255.0*/), sgColor4f(color.r, color.g, color.b, color.a * 0.25/*34.0/255.0*/));
    sgDrawSetSmooth(SG_FALSE);
    drawRects(x, y, 0, sgColor4f(color.r, color.g, color.b, color.a));
    sgDrawColor4f(color.r, color.g, color.b, color.a);
    sgDrawRectangleWH(x - 16 + 8, y + 16 - 2, 19, 1, SG_TRUE);
    sgDrawSetSmooth(SG_TRUE);

    color = (sub->mode == 1) ? smask : umask;
    x = 1 * 32 + 16.0;
    drawBand(x, y, 0, 4, sgColor4f(color.r, color.g, color.b, color.a), sgColor4f(color.r, color.g, color.b, color.a));
    drawBand(x, y, 4, 4.666, sgColor4f(color.r, color.g, color.b, color.a * 162.0/255.0), sgColor4f(color.r, color.g, color.b, color.a * 162.0/255.0));
    c1 = 0.0;
    c2 = lerp((6.648 - 6.20325) / (9.304875 - 6.20325), 0.0, 70.0/255.0);
    drawBand(x, y, 6.20325, 6.648, sgColor4f(color.r, color.g, color.b, 0.0), sgColor4f(color.r, color.g, color.b, color.a * c2));
    c1 = lerp((7.523 - 6.20325) / (9.304875 - 6.20325), 0.0, 70.0/255.0);
    c2 = lerp((8.523 - 6.20325) / (9.304875 - 6.20325), 0.0, 70.0/255.0);
    drawBand(x, y, 7.523, 8.523, sgColor4f(color.r, color.g, color.b, color.a * c1), sgColor4f(color.r, color.g, color.b, color.a * c2));
    c1 = lerp((9.585 - 9.304875) / (12.4065 - 9.304875), 70.0/255.0, 0.0);
    c2 = lerp((10.460 - 9.304875) / (12.4065 - 9.304875), 70.0/255.0, 0.0);
    drawBand(x, y, 9.585, 10.460, sgColor4f(color.r, color.g, color.b, color.a * c1), sgColor4f(color.r, color.g, color.b, color.a * c2));
    c1 = lerp((11.616 - 9.304875) / (12.4065 - 9.304875), 70.0/255.0, 0.0);
    c2 = 0.0;
    drawBand(x, y, 11.616, 12.4065, sgColor4f(color.r, color.g, color.b, color.a * c1), sgColor4f(color.r, color.g, color.b, color.a * c2));
    sgDrawSetSmooth(SG_FALSE);
    drawRects(x, y, 1, sgColor4f(color.r, color.g, color.b, color.a));
    sgDrawColor4f(color.r, color.g, color.b, color.a);
    sgDrawBegin(SG_GRAPHICS_PRIMITIVE_TRIANGLE_STRIP);
        sgDrawVertex2f(x - 16 + 9, y + 16 - 3);
        sgDrawVertex2f(x - 16 + 9, y + 16 - 2);
        sgDrawVertex2f(x - 16 + 11, y + 16 - 3);
        sgDrawVertex2f(x - 16 + 11, y + 16 - 2);
        sgDrawVertex2f(x - 16 + 13, y + 16 - 5);
        sgDrawVertex2f(x - 16 + 13, y + 16 - 4);
        sgDrawVertex2f(x - 16 + 15, y + 16 - 3);
        sgDrawVertex2f(x - 16 + 15, y + 16 - 2);
        sgDrawVertex2f(x - 16 + 20, y + 16 - 3);
        sgDrawVertex2f(x - 16 + 21, y + 16 - 2);
        sgDrawVertex2f(x - 16 + 23, y + 16 - 9);
        sgDrawVertex2f(x - 16 + 23, y + 16 - 7);
        sgDrawVertex2f(x - 16 + 26, y + 16 - 3);
        sgDrawVertex2f(x - 16 + 25, y + 16 - 2);
        sgDrawVertex2f(x - 16 + 29, y + 16 - 3);
        sgDrawVertex2f(x - 16 + 29, y + 16 - 2);
    sgDrawEnd();
    sgDrawSetSmooth(SG_TRUE);

    color = (sub->mode == 2) ? smask : umask;
    x = 2 * 32 + 16.0;
    drawBand(x, y, 0, 4, sgColor4f(color.r, color.g, color.b, color.a), sgColor4f(color.r, color.g, color.b, color.a));
    drawBand(x, y, 4, 4.666, sgColor4f(color.r, color.g, color.b, color.a * 162.0/255.0), sgColor4f(color.r, color.g, color.b, color.a * 162.0/255.0));
    drawBands(x, y, bands2, sizeof(bands2) / sizeof(*bands2), sgColor4f(color.r, color.g, color.b, color.a), sgColor4f(color.r, color.g, color.b, color.a));
    sgDrawSetSmooth(SG_FALSE);
    drawRects(x, y, 2, sgColor4f(color.r, color.g, color.b, color.a));
    sgDrawColor4f(color.r, color.g, color.b, color.a);
    sgDrawRectangleWH(x - 16 + 8, y + 16 - 2, 19, 1, SG_TRUE);
    sgDrawSetSmooth(SG_TRUE);

    color = (sub->mode == 3) ? smask : umask;
    x = 3 * 32 + 16.0;
    drawBand(x, y, 0, 4, sgColor4f(color.r, color.g, color.b, color.a), sgColor4f(color.r, color.g, color.b, color.a));
    drawBand(x, y, 4, 4.666, sgColor4f(color.r, color.g, color.b, color.a * 162.0/255.0), sgColor4f(color.r, color.g, color.b, color.a * 162.0/255.0));
    drawBands(x, y, bands3, sizeof(bands3) / sizeof(*bands3), sgColor4f(color.r, color.g, color.b, color.a), sgColor4f(color.r, color.g, color.b, color.a));
    sgDrawSetSmooth(SG_FALSE);
    drawRects(x, y, 3, sgColor4f(color.r, color.g, color.b, color.a));
    sgDrawColor4f(color.r, color.g, color.b, color.a);
    sgDrawRectangleWH(x - 16 + 8, y + 16 - 2, 19, 1, SG_TRUE);
    sgDrawSetSmooth(SG_TRUE);

    sgDrawSetSmooth(SG_FALSE);
    color = smask;
    sgDrawColor4f(color.r, color.g, color.b, color.a);
    x = sub->mode * 32 + 16.0;
    sgDrawRectangleWH(x - 16.0, y - 16.0, 32.0, 32.0, SG_FALSE);
    sgDrawSetSmooth(SG_TRUE);

    sgViewportSet4i4f(viewport, 0, 0, 640, 480, pos.x - 640/viewzoom/2, pos.y - 480/viewzoom/2, 640/viewzoom, 480/viewzoom);
}
void subDrawHealth(SGEntity* entity)
{
    //Sub* sub = entity->data;

    SGVec2 pos;
    sgEntityGetPos(entity, &pos.x, &pos.y);

    sgViewportSet4i(viewport, 0, 0, 640, 480);

    SGColor fill = sgColor4f(0.0, 0.25, 0.5, 0.75);
    SGColor line = sgColor4f(0.0, 0.5, 0.75, 1.0);
    SGColor color;

    sgDrawSetSmooth(SG_FALSE);
    color = fill;
    sgDrawColor4f(color.r, color.g, color.b, color.a);
    sgDrawRectangleWH(0, 480 - 32 - 16, 32 * 4, 16, SG_TRUE);

    color = line;
    sgDrawColor4f(color.r, color.g, color.b, color.a);
    sgDrawRectangleWH(0, 480 - 32 - 16, 32 * 4, 16, SG_FALSE);
    sgDrawSetSmooth(SG_TRUE);

    sgViewportSet4i4f(viewport, 0, 0, 640, 480, pos.x - 640/viewzoom/2, pos.y - 480/viewzoom/2, 640/viewzoom, 480/viewzoom);
}

void SG_EXPORT subCreateBubble(SGEntity* entity, SGVec2 rpos, size_t chance)
{
    if(rand() % 100 >= chance)
        return;

    Sub* sub = entity->data;

    SGVec2 pos;
    sgEntityGetPos(entity, &pos.x, &pos.y);

    pos = sgVec2Add(pos, rpos);

    sub->bubbles[sub->tail].pos = pos;
    sub->bubbles[sub->tail].vel.x = frand2(-0.1, 0.1);
    sub->bubbles[sub->tail].vel.y = frand2(-0.2, 0.0);
    sub->bubbles[sub->tail].alpha = 1.0;
    sub->tail = (sub->tail + 1) % (sizeof(sub->bubbles) / sizeof(*sub->bubbles));
    if(sub->head == sub->tail)
        sub->head = (sub->head + 1) % (sizeof(sub->bubbles) / sizeof(*sub->bubbles));
}

void SG_EXPORT lcSubDestroy(SGEntity* entity)
{
    Sub* sub = entity->data;

    sgAudioSourceDestroy(sub->srcEngine);
    free(sub);
}
void SG_EXPORT lcSubCollision(SGEntity* entity, SGEntity* other, SGPhysicsCollision* coll)
{
    Sub* sub = entity->data;

    /*SGVec2 vel;
    sgPhysicsBodyGetVel(sub->body, &vel.x, &vel.y);*/

    SGVec2 impulse;
    SGVec2 normal;
    sgPhysicsCollisionGetImpulse(coll, &impulse.x, &impulse.y, SG_TRUE);
    //printf("COLL %f,%f\n", xi, yi);

    sgPhysicsCollisionGetNormal(coll, 0, &normal.x, &normal.y);

    float dot = sgVec2Dot(sgVec2Normalize(impulse), sgVec2Normalize(sub->vel));
    //sgAudioSourceSetVolume(sub->srcGrinding, fabs(dot));
}
void SG_EXPORT evSubMouseButtonLeftPress(SGEntity* entity)
{
    //SGVec2 mpos = getMousePos();


}
void SG_EXPORT evSubMouseButtonRightPress(SGEntity* entity)
{
    SGVec2 mpos = getMousePos();

    SGVec2 pos;
    sgEntityGetPos(entity, &pos.x, &pos.y);

    SGAudioSource* srcPing = sgAudioSourceCreate(1.0, 1.0, 1.0, SG_FALSE);
    sgAudioSourceQueueBuffer(srcPing, bufPing);
    sgAudioSourcePlay(srcPing);
    sgAudioSourceDestroyLazy(srcPing);

    pingCreate(pos, sgVec2SetLength(sgVec2Sub(mpos, pos), 5.0), 1.0, 64.0, NULL);
}
void SG_EXPORT evSubMouseWheel(SGEntity* entity, SGint wheel)
{
    SGint pwheel = sgMouseGetWheelPrev();

    if(wheel - pwheel < 0)
        viewzoom = SG_MAX(viewzoom - 0.1, 1.0);
    else
        viewzoom = SG_MIN(viewzoom + 0.1, 2.0);
}
void SG_EXPORT evSubKeyboardKeyPress(SGEntity* entity, SGenum key)
{
    Sub* sub = entity->data;

    switch(key)
    {
        case 'Q':
            sub->mode = (sub->mode + sizeof(radii) / sizeof(*radii) - 1) % (sizeof(radii) / sizeof(*radii));
            if(sub->mode == 1) // intermittent is not supported for now!
                sub->mode--;
            break;
        case 'E':
            sub->mode = (sub->mode + 1) % (sizeof(radii) / sizeof(*radii));
            if(sub->mode == 1) // intermittent is not supported for now!
                sub->mode++;
            break;
        default:
            break;
    }
}
void SG_EXPORT evSubKeyboardKey(SGEntity* entity, SGenum key)
{
    Sub* sub = entity->data;

    switch(key)
    {
        case 'W': sub->vel.y -= 0.05; break;
        case 'S': sub->vel.y += 0.05; break;
        case 'A': sub->vel.x -= 0.05; sub->orient = SG_FALSE; break;
        case 'D': sub->vel.x += 0.05; sub->orient = SG_TRUE;  break;
        default:
            return;
    }

    SGVec2 v;
    v.x = fabs(sub->vel.x);
    v.y = fabs(sub->vel.y);

    SGVec2 max = sgVec2f(0.75, TOPVSPEED);

    if(v.x > max.x)
        sub->vel.x = (sub->vel.x / v.x) * max.x;
    if(v.y > max.y)
        sub->vel.y = (sub->vel.y / v.y) * max.y;

    float xf = -1.0 + sub->orient * 2.0;
    SGVec2 rear = sgVec2f(-14.0 + frand2(-2.0, 2.0), 1.0 + frand2(-2.0, 2.0));
    rear.x *= xf;
    rear = sgVec2SetAngleRads(rear, sgVec2GetAngleRads(rear) + sub->angle);

    subCreateBubble(entity, rear, 25);
}
void SG_EXPORT evSubTick(SGEntity* entity)
{
    Sub* sub = entity->data;

    float factor = 1.0;
    switch(sub->mode)
    {
        case 0: factor = 0.25; break;
        case 1: factor = 1.0; break;
        case 2: factor = 0.5; break;
        case 3: factor = 0.75; break;
        default:
            break;
    }

    sgAudioSourceSetVolume(sub->srcEngine, 0.25 * factor + sgVec2GetLength(sub->vel) * factor);
    sgAudioSourceSetPitch(sub->srcEngine, 0.25 + sgVec2GetLength(sub->vel));

    SGVec2 pos;
    sgEntityGetPos(entity, &pos.x, &pos.y);

    lightEdges(pos, radii[sub->mode], 1.0/4.0);

    pos = sgVec2Add(pos, sub->vel);
    sgEntitySetPos(entity, pos.x, pos.y);

    float vdamping = 0.95;

    sub->vel = sgVec2SetLength(sub->vel, sgVec2GetLength(sub->vel) * vdamping);
    if(sgVec2GetLength(sub->vel) < 0.005)
        sub->vel = sgVec2f(0.0, 0.0);

    subCreateBubble(entity, sgVec2f(frand2(-12.0, 12.0), frand2(-4.0, 4.0)), 5); // 5% chance of a bubble creating if we're doing nothing!
}
void SG_EXPORT evSubDraw(SGEntity* entity)
{
    size_t i;

    Sub* sub = entity->data;

    SGVec2 pos;
    sgEntityGetPos(entity, &pos.x, &pos.y);

    sgViewportSet4i4f(viewport, 0, 0, 640, 480, pos.x - 640/viewzoom/2, pos.y - 480/viewzoom/2, 640/viewzoom, 480/viewzoom);

    sgDrawColor4f(0.0, 0.25, 0.5, 1.0);

    float aangle;
    float xf;
    SGVec2 v;

    // this looks really ugly with smoothing on...
    sgDrawSetSmooth(SG_FALSE);
    sgDrawBegin(SG_GRAPHICS_PRIMITIVE_TRIANGLE_FAN);
        aangle = 10.0 * SG_PI / 180.0 * sub->vel.y / TOPVSPEED;
        if(sub->angle > aangle)
            sub->angle -= SG_MIN(0.5 * SG_PI / 180.0, sub->angle - aangle);
        else if(sub->angle < aangle)
            sub->angle += SG_MIN(0.5 * SG_PI / 180.0, aangle - sub->angle);
        xf = -1.0 + sub->orient * 2.0;

        sgDrawVertex2f(pos.x, pos.y);
        for(i = 0; i < sizeof(subvects) / sizeof(*subvects); i++)
        {
            v = sgVec2SetAngleRads(subvects[i], sgVec2GetAngleRads(subvects[i]) + sub->angle);
            sgDrawVertex2f(pos.x + xf * v.x, pos.y + v.y);
        }
    sgDrawEnd();
    sgDrawSetSmooth(SG_TRUE);

    sgDrawColor4f(0.0, 0.5, 0.75, 0.5);

    sgDrawBegin(SG_GRAPHICS_PRIMITIVE_LINE_STRIP);
        aangle = 10.0 * SG_PI / 180.0 * sub->vel.y / TOPVSPEED;
        if(sub->angle > aangle)
            sub->angle -= SG_MIN(0.5 * SG_PI / 180.0, sub->angle - aangle);
        else if(sub->angle < aangle)
            sub->angle += SG_MIN(0.5 * SG_PI / 180.0, aangle - sub->angle);
        xf = -1.0 + sub->orient * 2.0;

        for(i = 0; i < sizeof(subvects) / sizeof(*subvects); i++)
        {
            v = sgVec2SetAngleRads(subvects[i], sgVec2GetAngleRads(subvects[i]) + sub->angle);
            sgDrawVertex2f(pos.x + xf * v.x, pos.y + v.y);
        }
    sgDrawEnd();

    /*sgDrawRectangleWH(pos.x - 10.0, pos.y - 4.0, 20.0, 8.0, SG_TRUE);
    sgDrawCircle(pos.x + 10.0, pos.y, 4, SG_TRUE);*/

    //sgDrawColor4f(0.0, 0.0, 1.0, 0.5);
    drawEArcRads(pos.x, pos.y, radii[sub->mode] * 0.5, radii[sub->mode] * 0.5, 0, 2 * SG_PI, SG_FALSE, SG_TRUE, sgColor4f(0.0, 0.5, 0.75, 0.5), sgColor4f(0.0, 0.5, 0.75, 0.0));
    //sgDrawCircle(pos.x, pos.y, 128, SG_TRUE);

    for(i = sub->head; i != sub->tail; i = (i + 1) % (sizeof(sub->bubbles) / sizeof(*sub->bubbles)))
    {
        if(sub->bubbles[i].alpha > 0.0)
        {
            drawEArcRads(sub->bubbles[i].pos.x, sub->bubbles[i].pos.y, 1.5, 1.5, 0, 2 * SG_PI, SG_FALSE, SG_TRUE, sgColor4f(0.0, 0.5, 0.75, 0.0), sgColor4f(0.0, 0.5, 0.75, sub->bubbles[i].alpha));
            sub->bubbles[i].pos = sgVec2Add(sub->bubbles[i].pos, sub->bubbles[i].vel);
            sub->bubbles[i].pos = sgVec2Add(sub->bubbles[i].pos, sgVec2f(0.0, -0.05));
            sub->bubbles[i].vel = sgVec2SetLength(sub->bubbles[i].vel, sgVec2GetLength(sub->bubbles[i].vel) * 0.99);
            sub->bubbles[i].alpha -= 1.0/128.0;
        }
    }

    subDrawMode(entity);
    subDrawHealth(entity);

    sgDrawColor4f(1.0, 1.0, 1.0, 1.0);

    /*sgPhysicsShapeDrawDBG(sub->shape);

    sgPhysicsBodySetVel(sub->body, sub->vel.x, sub->vel.y);*/
}

Sub* subCreate(SGVec2 pos)
{
    Sub* sub = malloc(sizeof(Sub));
    if(!sub)
        return NULL;
    sub->entity = sgEntityCreate(0.0);
    sub->entity->data = sub;
    sub->entity->lcDestroy = lcSubDestroy;
    sub->entity->lcCollision = lcSubCollision;
    sub->entity->evMouseButtonLeftPress = evSubMouseButtonLeftPress;
    sub->entity->evMouseButtonRightPress = evSubMouseButtonRightPress;
    sub->entity->evMouseWheel = evSubMouseWheel;
    sub->entity->evKeyboardKeyPress = evSubKeyboardKeyPress;
    sub->entity->evKeyboardKey = evSubKeyboardKey;
    sub->entity->evTick = evSubTick;
    sub->entity->evDraw = evSubDraw;
    sgEntitySetPos(sub->entity, pos.x, pos.y);

    sub->vel = sgVec2f(0.0, 0.0);
    sub->srcEngine = sgAudioSourceCreate(0.5, 0.0, 1.0, SG_TRUE);
    sgAudioSourceQueueBuffer(sub->srcEngine, bufEngine);
    sgAudioSourcePlay(sub->srcEngine);

    sub->srcGrinding = sgAudioSourceCreate(1.0, 0.0, 0.75, SG_TRUE);
    sgAudioSourceQueueBuffer(sub->srcGrinding, bufGrinding);
    sgAudioSourcePlay(sub->srcGrinding);

    sub->head = 0;
    sub->tail = 0;

    sub->orient = SG_TRUE;
    sub->mode = sizeof(radii) / sizeof(*radii) - 1;

    sub->angle = 0.0;

    /*sub->body = sgPhysicsBodyCreate(space, SG_PHYSICS_BODY_NORMAL);
    sgPhysicsBodySetPos(sub->body, pos.x, pos.y);

    SGVec2 verts[4];
    verts[0] = sgVec2f(-12.0, -3.0);
    verts[1] = sgVec2f(-12.0, +4.0);
    verts[2] = sgVec2f(+12.0, +4.0);
    verts[3] = sgVec2f(+12.0, -3.0);

    //sub->shape = sgPhysicsShapeCreateCircle(sub->body, 0.0, 0.0, 4.0, 4.0);
    sub->shape = sgPhysicsShapeCreatePoly(sub->body, 0.0, 0.0, (float*)verts, 4);
    sgPhysicsShapeSetRestitution(sub->shape, 0.25);
    sgPhysicsShapeSetFriction(sub->shape, 0.75);

    sgPhysicsBodySetMass(sub->body, 1.0);
    sgPhysicsBodySetMoment(sub->body, sgPhysicsShapeGetMomentMass(sub->shape, 1.0));

    sgEntitySetPhysicsBody(sub->entity, sub->body);*/

    return sub;
}
