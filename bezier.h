#ifndef __BEZIER_H__
#define __BEZIER_H__

#include "common.h"

SGVec2 bezierPoint(const SGVec2* points, size_t numpoints, float t);
void bezierSample(SGVec2* samples, size_t numsamples, const SGVec2* points, size_t numpoints);

#endif // __BEZIER_H__
