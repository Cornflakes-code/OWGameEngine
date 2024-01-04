#include <algorithm>

#include <Windows.h>

#include "rope_quick.h"
#include "rope_interface_utils.h"

Quick_draw quick_draw = nullptr;
Quick_draw_int quick_draw_int = nullptr;
Quick_start quick_start = nullptr;
RopeBuf* pRCDefaults = nullptr;

bool initQuickExterns()
{
	HMODULE h = dllHandle();
	if (!h)
		return false;
	quick_draw = (Quick_draw)GetProcAddress(h, "quick_draw");
	if (quick_draw == nullptr)
		return false;
	quick_draw_int = (Quick_draw_int)GetProcAddress(h, "quick_draw_int");
	if (quick_draw_int == nullptr)
		return false;
	quick_start = (Quick_start)GetProcAddress(h, "quick_start");
	if (quick_start == nullptr)
		return false;
	std::string connectionString = "Data Source=NEXUS\\ROPECAD;Initial Catalog=RopeCAD;MultipleActiveResultSets=True;Integrated Security=True";
	RopeBuf* buffer = nullptr;
	RopeBuf* p = toRemoteString(connectionString, buffer);
	pRCDefaults = quick_start(p);
	return pRCDefaults != nullptr;
}

RopeBuf* calcQuickRope(int ropeNum, int width, int height)
{
	return quick_draw_int(ropeNum, width, height);
}

bool testQuickExterns()
{
	if (pRCDefaults != nullptr)
	{
		RopeBuf* rb = nullptr;
		//RopeBuf* con = toRemoteString("6x19(9//9//1)//P", rb);
		//RopeBuf* con = toRemoteString("7x7 WRC", rb);
		//result = quick_draw(con);
		RopeBuf* pDrawStructure = quick_draw_int(9239, 5, 5);
		PolygonBuilder pb;
		pb.populate(pDrawStructure);
	}
	return true;
}