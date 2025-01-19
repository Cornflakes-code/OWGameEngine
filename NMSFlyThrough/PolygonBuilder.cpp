#include "PolygonBuilder.h"

#include <Core/ErrorHandling.h>
#include <Core/LogStream.h>
#include <Core/CommonUtils.h>
#include <Geometry/BoundingBox.h>

void PolygonBuilder::clear()
{
	constexpr float fmax = std::numeric_limits<float>::max();
	constexpr float fmin = -std::numeric_limits<float>::max();
	mMinMax = { {fmin, fmin, fmin}, {fmax, fmax, fmax} };
	mAllPolygons = std::vector<std::vector<std::vector<glm::vec3>>>();
	mAllSliceLabels = std::vector<SliceId>();
}

void PolygonBuilder::get(RopeBuf* buffer)
{
	/*
	* allWires is a vector of wires
	* a wire is a collection of slices at various depths with z = 0.0 being first
	* a slice is a label, a Polygon type and a vector of points.
	*/
	std::vector<std::vector<WireSlice>> allWires;

	// Recursively populate allWires
	doPopulate(buffer, allWires);
	// Now separate the data out into two structure.
	// One is a 2D vectors of Polygons (the second dimension contains the depth Polygons for the wire)
	// The another is a 1D structure containing the id and id location of the first slice for each wire.
	// 
	bool boundsFound = false;
	for (std::vector<WireSlice>& wire: allWires)
	{
		std::vector<std::vector<glm::vec3>> layer;
		for (int i = 0; i < wire.size(); i++)
		{
			if (true)//i == 0) // Only label the first layer

			{
				AABB sliceBounds = AABB(wire[i].f);
				SliceId si;
				si.id = wire[i].id;
				si.pos = sliceBounds.center();
				mAllSliceLabels.push_back(si);

				if (!boundsFound)
				{
					boundsFound = true;
					mMinMax.first = sliceBounds.minPoint();
					mMinMax.second = sliceBounds.maxPoint();
				}
			}
			layer.push_back(wire[i].f);
		}
		mAllPolygons.push_back(layer);
	}
}

// floatBuf contains 2D points
void getPoints(std::vector<glm::vec3>& ff, RopeBuf* floatBuf, float zdepth)
{
	int numFloats = (int)(*floatBuf++);

	// Get the pointer to numFloats of float pairs
	float* f = (float*)(floatBuf);
	for (int i = 0; i < numFloats; i += 2)
	{
		float x = *f++;
		float y = *f++;
		float z = zdepth * 10;
		ff.push_back({ x, y, z });
	}
}

bool found701 = false;

// The buffer structure is determined by rope.cs
void PolygonBuilder::doPopulate(RopeBuf* buffer, std::vector<std::vector<WireSlice>>& allWires)
{
	RopeBuf* p = buffer;
	int numPolys = *((int*)(p++));
	int jfw = *((int*)(p++)); // needed to extract debug var in C# RCPolygonBuilder.getMemory()
	for (int i = 0; i < numPolys; i++)
	{
		RopeBuf* elm = (RopeBuf*)(*p++);
		doPopulate(elm, allWires); // Recurse into the data structure
	}

	if (found701)
		return;
	int numValues = (int)(*p++);
	for (int i = 0; i < numValues; i++)
	{
		float zdepth = *((float*)p++);
		RopeBuf* pf = (RopeBuf*)(*p++);
		std::vector<glm::vec3> points;
		getPoints(points, pf, zdepth);
		PolyType pt = (PolyType)(*p++);
		int id = (int)(*p++);

		//		if (!((id == 2055) || (id == 2002) || (id == 1000) || (id == 0)))
		//			continue;
		//if (!(id == 2056)) // complex rope
		//	continue;
		//if (!(id == 1001)) // simple rope
		//	continue;
		//found701 = true;
		WireSlice vt(points, pt, id);
		static float zOldDepth = std::numeric_limits<float>::max();
		if (zdepth != zOldDepth)
		{
			zOldDepth = zdepth;
			std::vector<WireSlice> temp;
			temp.push_back(vt);
			allWires.push_back(temp);
		}
		else
		{
			allWires.back().push_back(vt);
		}
		/*
		if (OWUtils::isEqual(zdepth, 0))
		{
			// The zero depth element is always the first polygon for a new wire
			std::vector<WireSlice> temp;
			temp.push_back(vt);
			allWires.push_back(temp);
		}
		else
		{
			if (!allWires.size())
				throw NMSException(std::stringstream()
					<< "PolygonBuilder::doPopulate. "
					<< "Did not expect empty allWires with non zero depth. zdepth was["
					<< zdepth << "].\n");
			allWires.back().push_back(vt);
		}
*/
	}
}

void PolygonBuilder::foreach(foreachPolyBuilder OW_UNUSED(fe))
{
/*
	for (auto& val : mPolygons)
	{
		val.foreach(fe);
	}
	for (auto& val : mValues)
	{
		//(*fe)(val.first);
	}
*/
}

