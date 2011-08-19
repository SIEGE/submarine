#ifndef __SUB_H__
#define __SUB_H__

#include "common.h"

typedef struct Bubble
{
    SGVec2 pos;
    SGVec2 vel;
    float alpha;
} Bubble;

#define TOPVSPEED 0.5

typedef struct Sub
{
    SGEntity* entity;
    SGVec2 vel;
    SGAudioSource* srcEngine;

    size_t head;
    size_t tail;
    Bubble bubbles[256];

    SGbool orient;
    SGubyte mode;

    float angle;
} Sub;
SGVec2 subvects[90];
SGVec2 subtail[2][9];
float radii[4];

void SG_EXPORT subCreateBubble(SGEntity* entity, SGVec2 rpos, size_t chance);

void SG_EXPORT lcSubDestroy(SGEntity* entity);
void SG_EXPORT evSubMouseButtonLeftPress(SGEntity* entity);
void SG_EXPORT evSubMouseButtonRightPress(SGEntity* entity);
void SG_EXPORT evSubMouseWheel(SGEntity* entity, SGint wheel);
void SG_EXPORT evSubKeyboardKey(SGEntity* entity, SGenum key);
void SG_EXPORT evSubTick(SGEntity* entity);
void SG_EXPORT evSubDraw(SGEntity* entity);

Sub* subCreate(SGVec2 pos);

#endif // __SUB_H__
