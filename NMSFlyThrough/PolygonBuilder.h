#pragma once
#include <vector>
#include <glm/glm.hpp>

typedef std::uintptr_t RopeBuf;

typedef std::vector<glm::vec3> Floats;
class PolygonBuilder;
typedef int (*foreachPolyBuilder) (PolygonBuilder&, std::vector<Floats>&);

class PolygonBuilder
{
	enum PolyType
	{
		RCProduct, RCLayer, RCWire, RCNonWire, RCUnknown
	};

	std::vector<PolygonBuilder> mPolygons;
	std::vector<std::pair<Floats, PolyType>> mValues;
	void getFloats(Floats& f, RopeBuf* floatBuf);
public:
	void getAllFloats(std::vector<Floats>& ff);
	void populate(RopeBuf* buffer);
	void foreach(foreachPolyBuilder fe);
};
