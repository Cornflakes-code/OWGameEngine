#pragma once
#include "rope_interface_utils.h"

typedef RopeBuf* (__cdecl* Quick_draw)(RopeBuf* construction);
typedef RopeBuf* (__cdecl* Quick_draw_int)(int prodId, int width, int height, int numDepthLayers);
typedef RopeBuf* (__cdecl* Quick_start)(RopeBuf* connectionString);

bool initQuickExterns();
bool testQuickExterns();
RopeBuf* calcQuickRope(int ropeNum, int width, int height, int numDepthLayers);
