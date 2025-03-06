#include "Mesh.h"
#include "../Geometry/BoundingBox.h"

AABB InstanceData::bounds() const
{
	if (v3.size())
	{
		return AABB(v3);
	}
	else if (v4.size())
	{
		return AABB(v4);
	}
	else
	{
		throw NMSLogicException("Data for InstanceData not set.");
	}
}

AABB MeshData::bounds() const
{
	if (v3.size())
	{
		return AABB(v3);
	}
	else if (v4.size())
	{
		return AABB(v4);
	}
	else
	{
		throw NMSLogicException("Data for MeshData not set.");
	}
}

