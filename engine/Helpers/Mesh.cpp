#include "Mesh.h"
#include "../Geometry/BoundingBox.h"

AABB MeshData::bounds() const
{
	if (mVec3.size())
	{
		return AABB(mVec3);
	}
	else if (mVec4.size())
	{
		return AABB(mVec4);
	}
	else
	{
		throw NMSLogicException("Data for MeshDataLight not set.");
	}
}

