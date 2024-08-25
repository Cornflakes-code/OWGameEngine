#include "PolygonBuilder.h"

glm::vec3 PolygonBuilder::center(const glm::vec3& p1, const glm::vec3& p2)
{
	return glm::vec3 ((p1.x + p2.x) / 2.0f,
					(p1.y + p2.y) / 2.0f,
					(p1.z + p2.z) / 2.0f);
}

std::pair<glm::vec3, glm::vec3> PolygonBuilder::boundingBox(const Floats& points)
{
	std::pair<glm::vec3, glm::vec3> retval =
	{
		glm::vec3(std::numeric_limits<float>::max(),
				std::numeric_limits<float>::max(),
				std::numeric_limits<float>::max()),
		glm::vec3(std::numeric_limits<float>::min(),
				std::numeric_limits<float>::min(),
				std::numeric_limits<float>::min())
	};
	for (const glm::vec3& pt : points)
	{
		if (pt.x < retval.first.x)
			retval.first.x = pt.x;
		if (pt.x > retval.second.x)
			retval.second.x = pt.x;

		if (pt.y < retval.first.y)
			retval.first.y = pt.y;
		if (pt.y > retval.second.y)
			retval.second.y = pt.y;

		if (pt.z < retval.first.z)
			retval.first.z = pt.z;
		if (pt.z > retval.second.z)
			retval.second.z = pt.z;
	}
	return retval;
}

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
		p += 1; // read zed here
		p += 1;
		int id = (int)(*p);
		ValueType vt(ff, pt, id);
		mValues.push_back(vt);
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
		const float z = 0;
		ff.push_back({ x, y, z });
	}
}

void PolygonBuilder::getAllFloats(std::vector<Floats>& ff, std::vector<PolygonId>& ids)
{
	for (auto& val : mPolygons)
	{
		val.getAllFloats(ff, ids);
	}
	for (auto& val : mValues)
	{
		ff.push_back(val.f);
		std::pair<glm::vec3, glm::vec3> boundsOfPoly = PolygonBuilder::boundingBox(val.f);

		PolygonId id;
		id.pos = center(boundsOfPoly.first, boundsOfPoly.second);
		id.id = val.id;
		ids.push_back(id);
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

