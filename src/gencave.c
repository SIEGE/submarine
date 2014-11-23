#include "gencave.h"

#include "fsiege.h"

#include <string.h>

void _caveIteratePCN(Cave* cave)
{
    size_t tmp = cave->next;
    cave->next = cave->prev;
    cave->prev = cave->curr;
    cave->curr = tmp;
}
size_t _caveNumWalls(Cave* cave, size_t idx, size_t i, size_t j)
{
    return cave->tiles[idx][i][j-1] + cave->tiles[idx][i-1][j] + cave->tiles[idx][i][j+1] + cave->tiles[idx][i+1][j]
         + cave->tiles[idx][i-1][j-1] + cave->tiles[idx][i+1][j-1] + cave->tiles[idx][i-1][j+1] + cave->tiles[idx][i+1][j+1];
}
void _caveDrawLine(Cave* cave, int x1, int y1, int x2, int y2, size_t set, size_t max)
{
    int dx = abs(x2-x1);
    int dy = abs(y2-y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int e2;

    while(1)
    {
        if(cave->tilesets[x1][y1] && cave->tilesets[x1][y1] != set)
            break;
        cave->tiles[cave->next][x1][y1] = SG_FALSE;
        cave->tilesets[x1][y1] = set;
        if(x1 == x2 && y1 == y2)
            break;

        if(cave->diag)
        {
            e2 = 2*err;
            if(e2 > -dy)
            {
                err -= dy;
                x1 += sx;
            }
            if(e2 < dx)
            {
                err += dx;
                y1 += sy;
            }
        }
        else
        {
            if(dx > dy) // we move in x direction
            {
                x1 += sx;
                dx = abs(x2-x1);
            }
            else
            {
                y1 += sy;
                dy = abs(x2-x1);
            }
        }
    }
}

SGbool _caveConnectTileTest(Cave* cave, size_t i, size_t j, void* data)
{
    return cave->tilesets[i][j] == *(size_t*)data;
}
SGbool _caveCalcSetsTest(Cave* cave, size_t i, size_t j, void* data)
{
    return !cave->tiles[cave->next][i][j] && !cave->tilesets[i][j];
}

void _caveConnectTile(Cave* cave, size_t max, size_t i, size_t j)
{
    size_t set = cave->tilesets[i][j];
    if(!set || set == max)
        return;

    _caveDrawLine(cave, i, j, cave->width / 2, cave->height / 2, set, max);
    _caveFloodFill(cave, i, j, max, _caveConnectTileTest, &set);
}
void _caveFloodFill(Cave* cave, size_t i, size_t j, size_t set, SGbool (*test)(Cave* cave, size_t i, size_t j, void* data), void* data)
{
    size_t fset = cave->tilesets[i][j];

    size_t top = 0;
    size_t* stack = malloc(2 * cave->width * cave->height * sizeof(size_t));

#define STEST(x,y) test(cave, stack[2*top+0]+(x), stack[2*top+1]+(y), data)
#define ADDITEM(x,y)    \
    do {    \
        if(fset)    \
            cave->setstr[fset]--; \
        cave->setstr[set]++;   \
        cave->tilesets[stack[2*top+0]+(x)][stack[2*top+1]+(y)] = set;    \
        top++;  \
        stack[2*top+0] = stack[2*top-2+0]+(x);    \
        stack[2*top+1] = stack[2*top-2+1]+(y);    \
    } while(0)

    stack[2*top+0] = i;
    stack[2*top+1] = j;

    ADDITEM(0,0);
    while(top)
    {
        if(cave->diag && STEST(-1,-1))
            ADDITEM(-1,-1);
        else if(STEST(-1, 0))
            ADDITEM(-1, 0);
        else if(cave->diag && STEST(-1, 1))
            ADDITEM(-1, 1);
        else if(STEST( 0,-1))
            ADDITEM( 0,-1);
        /*else if(STEST( 0, 0))
            ADDITEM( 0, 0);*/
        else if(STEST( 0, 1))
            ADDITEM( 0, 1);
        else if(cave->diag && STEST( 1,-1))
            ADDITEM( 1,-1);
        else if(STEST( 1, 0))
            ADDITEM( 1, 0);
        else if(cave->diag && STEST( 1, 1))
            ADDITEM( 1, 1);
        else
            top--;
    }
    free(stack);
#undef STEST
#undef ADDITEM
}

void _caveMarchTile(Cave* cave, size_t i, size_t j, SGubyte map, float xo, float yo, float xs, float ys, CaveMarch* march, void* data)
{
    float x = xo + i * xs;
    float y = yo + j * ys;

    switch(map)
    {
        case 0x00:
        case 0x0F: break;
        case 0x01:
        case 0x0E: march(cave, i, j, map, x, y + ys / 2.0, x + xs / 2.0, y + ys, data); break;
        case 0x02:
        case 0x0D: march(cave, i, j, map, x + xs / 2.0, y + ys, x + xs, y + ys / 2.0, data); break;
        case 0x03:
        case 0x0C: march(cave, i, j, map, x, y + ys / 2.0, x + xs, y + ys / 2.0, data); break;
        case 0x04:
        case 0x0B: march(cave, i, j, map, x + xs / 2.0, y, x + xs, y + ys / 2.0, data); break;
        case 0x05: // TODO: should we pick other diagonals?
            if(cave->diag)
            {
                march(cave, i, j, map, x, y + ys / 2.0, x + xs / 2.0, y + ys, data);
                march(cave, i, j, map, x + xs / 2.0, y, x + xs, y + ys / 2.0, data);
            }
            else
            {
                march(cave, i, j, map, x, y + ys / 2.0, x + xs / 2.0, y, data);
                march(cave, i, j, map, x + xs / 2.0, y + ys, x + xs, y + ys / 2.0, data);
            }
            break;
        case 0x06:
        case 0x09: march(cave, i, j, map, x + xs / 2.0, y, x + xs / 2.0, y + ys, data); break;
        case 0x07:
        case 0x08: march(cave, i, j, map, x, y + ys / 2.0, x + xs / 2.0, y, data); break;
        case 0x0A: // TODO: should we pick other diagonals?
            if(cave->diag)
            {
                march(cave, i, j, map, x, y + ys / 2.0, x + xs / 2.0, y, data);
                march(cave, i, j, map, x + xs / 2.0, y + ys, x + xs, y + ys / 2.0, data);
            }
            else
            {
                march(cave, i, j, map, x, y + ys / 2.0, x + xs / 2.0, y + ys, data);
                march(cave, i, j, map, x + xs / 2.0, y, x + xs, y + ys / 2.0, data);
            }
            break;
    }
}

Cave* caveCreate(size_t width, size_t height, SGbool diag)
{
    Cave* cave = malloc(sizeof(Cave));
    if(!cave)
        return NULL;

    cave->width = width;
    cave->height = height;
    cave->diag = diag;

    size_t i, k;
    for(k = 0; k < 3; k++)
    {
        cave->tiles[k] = malloc(width * sizeof(SGbool*));
        for(i = 0; i < width; i++)
            cave->tiles[k][i] = malloc(height * sizeof(SGbool));
    }
    cave->tilesets = malloc(width * sizeof(size_t*));
    for(i = 0; i < width; i++)
        cave->tilesets[i] = malloc(height * sizeof(size_t));
    cave->setstr = malloc(width * height * sizeof(size_t));

    return cave;
}
void caveDestroy(Cave* cave)
{
    if(!cave)
        return;

    size_t i, k;
    for(k = 0; k < 3; k++)
    {
        for(i = 0; i < cave->width; i++)
            free(cave->tiles[k][i]);
        free(cave->tiles[k]);
    }
    for(i = 0; i < cave->width; i++)
        free(cave->tilesets[i]);
    free(cave->tilesets);
    free(cave->setstr);

    free(cave);
}

void caveSeedTiles(Cave* cave, size_t rate)
{
    cave->prev = 0;
    cave->curr = 1;
    cave->next = 2;

    size_t i, j;
    for(i = 0; i < cave->width; i++)
    {
        for(j = 0; j < cave->height; j++)
        {
            if((i == 0 || i == cave->width - 1)
            || (j == 0 || j == cave->height - 1))
            {
                cave->tiles[cave->curr][i][j] = SG_TRUE;
                continue;
            }

            cave->tiles[cave->curr][i][j] = rand() % 100 < rate;
        }
        // in case we have 0 steps, we need this!
        memcpy(cave->tiles[cave->prev][i], cave->tiles[cave->curr][i], cave->height * sizeof(SGbool));
        memcpy(cave->tiles[cave->next][i], cave->tiles[cave->curr][i], cave->height * sizeof(SGbool));
    }
}
void caveStepTiles(Cave* cave)
{
    size_t nwalls;

    size_t i, j;
    for(i = 0; i < cave->width; i++)
    {
        for(j = 0; j < cave->height; j++)
        {
            if((i == 0 || i == cave->width - 1)
            || (j == 0 || j == cave->height - 1))
            {
                cave->tiles[cave->curr][i][j] = SG_TRUE;
                continue;
            }

            nwalls = _caveNumWalls(cave, cave->curr, i, j);

            if(cave->tiles[cave->curr][i][j] && nwalls >= 4)
                cave->tiles[cave->next][i][j] = SG_TRUE;
            else if(!cave->tiles[cave->curr][i][j] && nwalls >= 5)
                cave->tiles[cave->next][i][j] = SG_TRUE;
            else
                cave->tiles[cave->next][i][j] = SG_FALSE;
        }
    }

    _caveIteratePCN(cave);
}

void caveCalcSets(Cave* cave)
{
    size_t i, j;
    for(i = 0; i < cave->width; i++)
        memset(cave->tilesets[i], 0, cave->height * sizeof(size_t));
    memset(cave->setstr, 0, cave->width * cave->height * sizeof(size_t));

    cave->maxset = 0;

    for(i = 0; i < cave->width; i++)
    {
        for(j = 0; j < cave->height; j++)
        {
            /*if((i == 0 || i == cave->width - 1)
            || (j == 0 || j == cave->height - 1))
                continue;*/

            if(!cave->tiles[cave->next][i][j] && !cave->tilesets[i][j])
                _caveFloodFill(cave, i, j, ++cave->maxset, _caveCalcSetsTest, NULL);
        }
    }
}
void caveConnectTiles(Cave* cave)
{
    // nothing to do - we already have only 1 set!
    if(cave->maxset <= 1)
        return;

    size_t maxstr = 1;

    size_t i, j;
    for(i = 2; i <= cave->maxset; i++)
        if(cave->setstr[maxstr] < cave->setstr[i])
            maxstr = i;

    size_t* last = malloc(2 * (cave->maxset + 1) * sizeof(size_t));
    memset(last, 0, 2 * (cave->maxset + 1) * sizeof(size_t));

    for(i = 0; i < cave->width; i++)
    {
        for(j = 0; j < cave->height; j++)
        {
            if((i == 0 || i == cave->width - 1)
            || (j == 0 || j == cave->height - 1))
                continue;

            if(cave->tilesets[i][j] && cave->tilesets[i][j] != maxstr)
            {
                if(frand2(0.0, 1.0) < 1.0 / cave->setstr[cave->tilesets[i][j]])
                {
                    _caveConnectTile(cave, maxstr, i, j);
                    last[2*cave->tilesets[i][j]+0] = 0;
                    last[2*cave->tilesets[i][j]+1] = 0;
                }
                else
                {
                    last[2*cave->tilesets[i][j]+0] = i;
                    last[2*cave->tilesets[i][j]+1] = j;
                }
            }
        }
    }

    for(i = 1; i <= cave->maxset; i++)
    {
        if(last[2*i])
            _caveConnectTile(cave, maxstr, last[2*i+0], last[2*i+1]);
    }

    free(last);
}

void caveMarchTiles(Cave* cave, float xo, float yo, float xs, float ys, CaveMarch* march, void* data)
{
    SGubyte map;
    size_t i, j;
    for(i = 0; i < cave->width; i++)
    {
        for(j = 0; j < cave->height; j++)
        {
            if((/*i == 0 ||*/ i == cave->width - 1)
            || (/*j == 0 ||*/ j == cave->height - 1))
            {
                //tiles[curr][i][j] = SG_TRUE;
                continue;
            }

            map = (cave->tiles[cave->next][i  ][j+1] << 0)
                | (cave->tiles[cave->next][i+1][j+1] << 1)
                | (cave->tiles[cave->next][i+1][j  ] << 2)
                | (cave->tiles[cave->next][i  ][j  ] << 3);
            _caveMarchTile(cave, i, j, map, xo, yo, xs, ys, march, data);
        }
    }
}

void caveDrawTilesDBG(Cave* cave, float xo, float yo, float xs, float ys)
{
    size_t i, j;
    for(i = 0; i < cave->width; i++)
    {
        for(j = 0; j < cave->height; j++)
        {
            if(cave->tiles[cave->next][i][j])
                sgDrawRectangle2fWH(xo + i * xs, yo + j * ys, xs, ys, SG_TRUE);
            sgDrawColor4f(1.0, 0.0, 0.0, 1.0);
            sgFontPrintAlignedf(font, xo + i * xs + xs / 2, yo + j * ys + ys/2, SG_ALIGN_CENTER, "%lu", (unsigned long)cave->tilesets[i][j]);
            sgDrawColor4f(1.0, 1.0, 1.0, 1.0);
        }
    }
}
