#include "Ropes.h"
#include <string>
#include "./../NMSFlyThrough/rope_interface_test.h"
#include "./../NMSFlyThrough/rope_quick.h"


bool createRopes()
{
	bool ok = initInterfaceUtils() && initTestFunctions() && initQuickExterns();
	if (ok)
	{
		ok = runImportTests();
		if (ok)
		{
			RopeBuf* pDrawStructure = calcQuickRope(9239, 5, 5);
			PolygonBuilder pb;
			pb.populate(pDrawStructure);
		}
	}
	return ok;
}

PolygonBuilder* drawRope(int ropeNum, int width, int height)
{
	RopeBuf* pDrawStructure = calcQuickRope(ropeNum, width, height);
	PolygonBuilder* pb = new PolygonBuilder();
	pb->populate(pDrawStructure);
	return pb;
}
