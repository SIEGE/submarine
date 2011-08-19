#ifndef __GENCAVE_H__
#define __GENCAVE_H__

#include "common.h"

typedef struct Cave
{
    size_t width, height;
    SGbool diag;
    SGbool** tiles[3];
    size_t** tilesets;
    size_t* setstr;
    size_t maxset;
    size_t prev, curr, next;
} Cave;

typedef void (CaveMarch)(Cave* cave, size_t i, size_t j, SGubyte map, float x1, float y1, float x2, float y2, void* data);

void _caveIteratePCN(Cave* cave);
size_t _caveNumWalls(Cave* cave, size_t idx, size_t i, size_t j);
void _caveDrawLine(Cave* cave, int x1, int y1, int x2, int y2, size_t set, size_t max);

SGbool _caveConnectTileTest(Cave* cave, size_t i, size_t j, void* data);
SGbool _caveCalcSetsTest(Cave* cave, size_t i, size_t j, void* data);

void _caveConnectTile(Cave* cave, size_t max, size_t i, size_t j);
void _caveFloodFill(Cave* cave, size_t i, size_t j, size_t set, SGbool (*test)(Cave* cave, size_t i, size_t j, void* data), void* data);

void _caveMarchTile(Cave* cave, size_t i, size_t j, SGubyte map, float xo, float yo, float xs, float ys, CaveMarch* march, void* data);

Cave* caveCreate(size_t width, size_t height, SGbool diag);
void caveDestroy(Cave* cave);

void caveSeedTiles(Cave* cave, size_t rate);
void caveStepTiles(Cave* cave);
void caveCalcSets(Cave* cave);
void caveConnectTiles(Cave* cave);

void caveMarchTiles(Cave* cave, float xo, float yo, float xs, float ys, CaveMarch* march, void* data);

void caveDrawTilesDBG(Cave* cave, float xo, float yo, float xs, float ys);

#endif // __GENCAVE_H__
