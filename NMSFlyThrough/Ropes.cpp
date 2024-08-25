#include "Ropes.h"
#include <string>
#include "./../NMSFlyThrough/rope_interface_test.h"
#include "./../NMSFlyThrough/rope_quick.h"


bool Rope::initRopes()
{
	bool ok = initInterfaceUtils() && initTestFunctions() && initQuickExterns();
	if (ok)
	{
		ok = runImportTests();
	}
	return ok;
}

PolygonBuilder* Rope::drawRope(int ropeNum, int width, int height)
{
	RopeBuf* pointSourceBuffer = calcQuickRope(ropeNum, width, height);
	PolygonBuilder* pb = new PolygonBuilder();
	pb->populate(pointSourceBuffer);
	return pb;
}
