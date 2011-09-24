#include "bezier.h"

#include "fsiege.h"

#include <string.h>

SGVec2 bezierPoint(const SGVec2* points, size_t numpoints, float t)
{
    SGVec2 ret;
    SGVec2* buf = malloc(numpoints * sizeof(SGVec2));
    memcpy(buf, points, numpoints * sizeof(SGVec2));

    size_t i;
    while(numpoints > 1)
    {
        for(i = 0; i < numpoints - 1; i++)
            buf[i] = lerp2(t, buf[i], buf[i+1]);
        numpoints--;
    }
    ret = buf[0];

    free(buf);
    return ret;
}

void bezierSample(SGVec2* samples, size_t numsamples, const SGVec2* points, size_t numpoints)
{
    size_t i;
    float t;
    for(i = 0; i < numsamples; i++)
    {
        t = i / (float)(numsamples - 1);
        samples[i] = bezierPoint(points, numpoints, t);
    }
}
