#include <siege/siege.h>

#include "common.h"
#include "fsiege.h"
#include "sub.h"
#include "edge.h"
#include "pedge.h"
#include "ping.h"
#include "bezier.h"
#include "gencave.h"
#include "torpedo.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

SGbool debugmode;

void cbMarchEdge(Cave* cave, size_t i, size_t j, SGubyte map, float x1, float y1, float x2, float y2, void* data)
{
    createEdge(edges, sgVec2f(x1, y1), sgVec2f(x2, y2));
    //sgDrawLine(x1, y1, x2, y2);
}
void cbMarchDraw(Cave* cave, size_t i, size_t j, SGubyte map, float x1, float y1, float x2, float y2, void* data)
{
    sgDrawLine(x1, y1, x2, y2);
}

void SG_EXPORT evKeyboardKeyPress(SGEntity* entity, SGenum key)
{
    if(key == SG_KEYBOARD_KEY_F1)
        debugmode = !debugmode;
}

int main()
{
    sgLoadModule("SDL");
    sgLoadModule("OpenGL");
    sgLoadModule("DevIL");
    sgLoadModule("OpenAL");
    sgLoadModule("SndFile");
    sgLoadModule("Freetype");
    sgLoadModule("Chipmunk");
    sgInit(0);
    sgWindowOpen(640, 480, 32, 0);

    size_t i;

    font = sgFontCreate("data/fonts/DejaVuSans.ttf", 8, 127);
    uifont = sgFontCreate("data/fonts/DejaVuSans.ttf", 12, 127);

    space = sgPhysicsSpaceGetDefault();
    sbody = sgPhysicsSpaceGetStaticBody(space);

    sgPhysicsSpaceSetGravity(space, 0.0, 0.0);
    sgPhysicsSpaceSetDamping(space, 0.95);

    bufMusic = sgAudioBufferCreateFile("data/audio/WagnerRideOfTheValkyries.ogg");
    bufPing = sgAudioBufferCreateFile("data/audio/ping.ogg");
    bufEngine = sgAudioBufferCreateFile("data/audio/engine.ogg");
    bufGrinding = sgAudioBufferCreateFile("data/audio/grinding.wav");

    srcMusic = sgAudioSourceCreate(0.0, 0.5, 0.25, SG_TRUE);
    sgAudioSourceQueueBuffer(srcMusic, bufMusic);
    sgAudioSourcePlay(srcMusic);

    viewport = sgViewportCreate4i4f(0, 0, 640, 480, 640/4, 480/4, 640/2, 480/2);
    sgViewportReset(viewport);
    viewzoom = 1.0;

    sgDrawSetSmooth(SG_TRUE);

    edges = sgListCreate();

    /*SGVec2 verts[7];
    size_t numverts = sizeof(verts) / sizeof(*verts);

    verts[0] = sgVec2f(64, 64);
    verts[1] = sgVec2f(64, 480 - 64);
    verts[2] = sgVec2f(640 - 64, 480 - 64);
    verts[3] = sgVec2f(640 - 64, 64);
    verts[4] = sgVec2f(640 / 2 + 640 / 4, 192);
    verts[5] = sgVec2f(640 / 2, 128);
    verts[6] = sgVec2f(640 / 2 - 640 / 4, 192);

    for(i = 0; i < numverts; i++)
        createEdge(edges, verts[i], verts[(i + 1) % numverts]);*/

    SGVec2 pos;
    //SGVec2 mpos;
    //SGVec2 norm;

    sgWindowSetFPSLimit(60.0);

    SGListNode* node;

    radii[0] = 0.0;
    radii[1] = 64.0;
    radii[2] = 64.0;
    radii[3] = 128.0;

    Sub* sub = subCreate(sgVec2f(640 / 4, 480 / 2));

    pos = sgVec2f(4.724266, 38.7138);
    pos = sgVec2f(-12.0, 4.0);
    //pos = sgVec2Add(pos, sgVec2f(200, 200));

    size_t nbezier = 0;
    SGVec2 bezier[28];
    bezier[nbezier++] = pos;

    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(18.426459,1.2251));
    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(24.422095,-0.4774));
    pos = sgVec2Add(pos, sgVec2f(24.422095,-0.4774));
    bezier[nbezier++] = pos;

    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(0,0));
    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(2.082606,-0.4369));
    pos = sgVec2Add(pos, sgVec2f(2.103165,-2.3863));
    bezier[nbezier++] = pos;

    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-0.453555,-2.8178));
    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-2.370754,-3.2652));
    pos = sgVec2Add(pos, sgVec2f(-2.754353,-3.3523));
    bezier[nbezier++] = pos;

    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-1.439087,-0.3938));
    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-6.393663,-1.0241));
    pos = sgVec2Add(pos, sgVec2f(-6.393663,-1.0241));
    bezier[nbezier++] = pos;

    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-0.590676,-3.2343));
    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-2.721972,-2.6833));
    pos = sgVec2Add(pos, sgVec2f(-5.858504,-2.7184));
    bezier[nbezier++] = pos;

    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-2.632832,0.18));
    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-3.554207,1.6324));
    pos = sgVec2Add(pos, sgVec2f(-4.473982,2.8813));
    bezier[nbezier++] = pos;

    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-0.534845,0.7262));
    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-3.5119771,1.1784));
    pos = sgVec2Add(pos, sgVec2f(-5.7156645,1.2691));
    bezier[nbezier++] = pos;

    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-0.5139591,0.1379));
    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(-3.0315773,-0.2484));
    pos = sgVec2Add(pos, sgVec2f(-2.9904606,2.7052));
    bezier[nbezier++] = pos;

    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(0.041116,3.0716));
    bezier[nbezier++] = sgVec2Add(pos, sgVec2f(1.6613671,3.1029));
    pos = sgVec2Add(pos, sgVec2f(1.6613671,3.1029));
    bezier[nbezier++] = pos;

    for(i = 0; i + 4 <= nbezier; i += 3)
        bezierSample(subvects + i / 3 * sizeof(subvects) / sizeof(*subvects) / 9, sizeof(subvects) / sizeof(*subvects) / 9, bezier + i, 4);

    /*Edge* edge;
    SGVec2 i1, i2;
    size_t numinter;*/

    size_t xtiles = 16;
    size_t ytiles = 16;
    float xscale = xtiles * 2.5;
    float yscale = ytiles * 2.5;

    Cave* cave = caveCreate(640 / xtiles, 480 / ytiles, SG_TRUE);

    srand(1);
    caveSeedTiles(cave, 40);

    for(i = 0; i < 3; i++)
        caveStepTiles(cave);

    caveCalcSets(cave);
    caveConnectTiles(cave);

    caveMarchTiles(cave, 0.0, 0.0, xscale, yscale, cbMarchEdge, NULL);

    debugmode = SG_FALSE;
    SGEntity* control = sgEntityCreate(0.0);
    control->evKeyboardKeyPress = evKeyboardKeyPress;

    while(sgLoop(NULL))
    {
        sgEntityGetPos(sub->entity, &pos.x, &pos.y);

        //mpos = getMousePos();
        //norm = sgVec2Normalize(sgVec2Sub(mpos, pos));

        if(debugmode)
            for(node = edges->first; node; node = node->next)
                drawEdgeDBG(node->item);

        /*sgDrawCircle(mpos.x, mpos.y, 64.0, SG_FALSE);
        for(node = edges->first; node; node = node->next)
        {
            edge = node->item;
            numinter = sgIntersectCS(mpos, 64.0, edge->head, edge->tail, &i1, &i2);

            if(numinter >= 1)
                sgDrawCircle(i1.x, i1.y, 4, SG_TRUE);
            if(numinter >= 2)
                sgDrawCircle(i2.x, i2.y, 4, SG_TRUE);
        }*/

        //reflectRay(pos, norm, sgColor4f(0.0, 1.0, 0.0, 1.0), 0, NULL);

        //sgDrawColor4f(1.0, 1.0, 1.0, 1.0);
        //caveDrawTilesDBG(cave, 0.0, 0.0, xscale, yscale);

        sgDrawColor4f(1.0, 0.0, 0.0, 1.0);
        //caveMarchTiles(cave, 0.0, 0.0, xscale, yscale, cbMarchDraw, NULL);

        sgDrawColor4f(1.0, 1.0, 1.0, 1.0);

        sgWindowSwapBuffers();
        sgDrawClear();
    }

    caveDestroy(cave);

    sgFontDestroy(uifont);
    sgFontDestroy(font);

    sgViewportDestroy(viewport);

    while(edges->first)
        destroyEdge(edges->first->item);
    sgListDestroy(edges);

    sgAudioSourceDestroy(srcMusic);

    sgAudioBufferDestroy(bufPing);
    sgAudioBufferDestroy(bufMusic);

    sgDeinit();

    return 0;
}
