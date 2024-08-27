#include "PolygonBuilder.h"

#include <Core/ErrorHandling.h>
#include <Core/LogStream.h>
#include <Core/CommonUtils.h>

glm::vec3 PolygonBuilder::center(const glm::vec3& p1, const glm::vec3& p2)
{
	return glm::vec3 ((p1.x + p2.x) / 2.0f,
					(p1.y + p2.y) / 2.0f,
					(p1.z + p2.z) / 2.0f);
}

std::pair<glm::vec3, glm::vec3> PolygonBuilder::boundingBox(const std::vector<glm::vec3>& points)
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

void PolygonBuilder::clear()
{
	const float fmin = std::numeric_limits<float>::min();
	const float fmax = std::numeric_limits<float>::max();
	mMinMax = { {fmax, fmax, fmax}, {fmin, fmin, fmin} };
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
			if (i == 0)
			{
				// Only label the first layer
				std::pair<glm::vec3, glm::vec3> sliceBounds = PolygonBuilder::boundingBox(wire[0].f);
				SliceId si;
				si.id = wire[0].id;
				si.pos = center(sliceBounds.first, sliceBounds.second);
				mAllSliceLabels.push_back(si);

				if (!boundsFound)
				{
					boundsFound = true;
					mMinMax = sliceBounds;
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
		float z = -zdepth;
		ff.push_back({ x, y, z });
	}
}

// The buffer structure is determined by rope.cs
void PolygonBuilder::doPopulate(RopeBuf* buffer, std::vector<std::vector<WireSlice>>& allWires)
{
	RopeBuf* p = buffer;
	int numPolys = *((int*)(p++));
	int jfw = *((int*)(p++));
	for (int i = 0; i < numPolys; i++)
	{
		RopeBuf* elm = (RopeBuf*)(*p++);
		doPopulate(elm, allWires); // Recurse into the data structure
	}

	int numPolygons = (int)(*p++);
	for (int i = 0; i < numPolygons; i++)
	{
		float zdepth = *((float*)p++);
		RopeBuf* pf = (RopeBuf*)(*p++);
		std::vector<glm::vec3> points;
		getPoints(points, pf, zdepth);
		PolyType pt = (PolyType)(*p++);
		int id = (int)(*p++);
		WireSlice vt(points, pt, id);
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

