#pragma once
#include "rope_interface_utils.h"
#include "PolygonBuilder.h"

namespace Rope
{
	bool initRopes();
	PolygonBuilder* drawRope(int ropeNum, int width, int height, int numDepthLayers);
}
