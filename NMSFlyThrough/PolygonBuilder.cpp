#include "PolygonBuilder.h"

void PolygonBuilder::populate(RopeBuf* buffer)
{
	//typedef std::vector<xyz> Floats;
	//std::vector<std::pair<Floats, PolyType>> mValues;
	RopeBuf* p = buffer;
	int numPolys = *((int*)(p));
	p += 1;
	int jfw = *((int*)(p));
	for (int i = 0; i < numPolys; i++)
	{
		p += 1;
		PolygonBuilder pb;
		RopeBuf* elm = (RopeBuf*)(*p);
		pb.populate(elm);
		mPolygons.push_back(pb);
	}
	p += 1;
	int numFloats = (int)(*p);
	for (int i = 0; i < numFloats; i++)
	{
		p += 1;
		RopeBuf* pf = (RopeBuf*)(*p);
		Floats ff;
		getFloats(ff, pf);
		p += 1;
		PolyType pt = (PolyType)(*p);
		mValues.push_back(std::make_pair(ff, pt));
	}
}

void PolygonBuilder::getFloats(Floats& ff, RopeBuf* floatBuf)
{
	int numFloats = (int)(*floatBuf);
	floatBuf += 1;// sizeof(RopeBuf);
	float* f = (float*)(floatBuf);
	for (int i = 0; i < numFloats; i += 2)
	{
		float x = *f;  f += 1;// sizeof(float);
		float y = *f;  f += 1;//sizeof(float);
		float z = 0;
		ff.push_back({ x, y, z });
	}
}

void PolygonBuilder::getAllFloats(std::vector<Floats>& ff)
{
	for (auto& val : mPolygons)
	{
		val.getAllFloats(ff);
	}
	for (auto& val : mValues)
	{
		ff.push_back(val.first);
	}
}

void PolygonBuilder::foreach(foreachPolyBuilder fe)
{
	for (auto& val : mPolygons)
	{
		val.foreach(fe);
	}
	for (auto& val : mValues)
	{
		//(*fe)(val.first);
	}
}

