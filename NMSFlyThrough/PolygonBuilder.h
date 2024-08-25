#pragma once
#include <string>
#include <vector>
#include <limits>
#include <glm/glm.hpp>

typedef std::uintptr_t RopeBuf;

struct PolygonId
{
	glm::vec3 pos;
	int id;
};

typedef std::vector<glm::vec3> Floats;
class PolygonBuilder;
typedef int (*foreachPolyBuilder) (PolygonBuilder&, std::vector<Floats>&);

class PolygonBuilder
{
	enum PolyType
	{
		RCProduct, RCLayer, RCWire, RCNonWire, RCUnknown
	};
	struct ValueType
	{
		ValueType(Floats _f, PolyType _pt, int _id)
			: f(_f), p(_pt), id(_id)
		{}
		Floats f;
		PolyType p;
		int id;
	};

	std::vector<PolygonBuilder> mPolygons;
	std::vector<ValueType> mValues;
	void getFloats(Floats& f, RopeBuf* floatBuf);
public:
	static std::pair<glm::vec3, glm::vec3> boundingBox(const Floats& points);
	static glm::vec3 center(const glm::vec3& p1, const glm::vec3& p2);
	void getAllFloats(std::vector<Floats>& ff, std::vector<PolygonId>& ids);
	void populate(RopeBuf* buffer);
	void foreach(foreachPolyBuilder fe);
};
