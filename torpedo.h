#ifndef __TORPEDO_H__
#define __TORPEDO_H__

#include "common.h"

typedef struct Torpedo
{
    SGEntity* entity;
    SGVec2 vel;

    SGbool orient;

    float angle;
} Torpedo;

#endif // __TORPEDO_H__
