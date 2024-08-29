#pragma once
#include <string>
#include <vector>
#include <limits>
#include <glm/glm.hpp>

typedef std::uintptr_t RopeBuf;

class PolygonBuilder;
typedef int (*foreachPolyBuilder) (PolygonBuilder&, std::vector<std::vector<glm::vec3>>&);

class PolygonBuilder
{
public:
	struct SliceId
	{
		glm::vec3 pos;
		int id;
	};
	enum PolyType
	{
		RCProduct, RCLayer, RCWire, RCNonWire, RCUnknown
	};
	struct WireSlice
	{
		WireSlice(const std::vector<glm::vec3>& _f, PolyType _pt, int _id)
			: f(_f), p(_pt), id(_id)
		{
		}
		std::vector<glm::vec3> f;
		PolyType p;
		int id;
	};
private:
	/*
	* PolygonBuilder contains a vector of PolygonBuilder. This actuarately reflects
	* the Rope structure passed back
	*/
	std::pair<glm::vec3, glm::vec3> mMinMax;
	std::vector<std::vector<std::vector<glm::vec3>>> mAllPolygons;
	std::vector<SliceId> mAllSliceLabels;
	void clear();
	void doPopulate(RopeBuf* buffer, std::vector<std::vector<WireSlice>>& allWires);
public:
	virtual ~PolygonBuilder()
	{
		clear();
	}
	void get(RopeBuf* buffer);
	void foreach(foreachPolyBuilder fe);
	const std::vector<SliceId>& labels() { return mAllSliceLabels; }
	std::vector<std::vector<std::vector<glm::vec3>>>& slices() { return mAllPolygons; }
	std::pair<glm::vec3, glm::vec3> bounds() { return mMinMax; }
};
