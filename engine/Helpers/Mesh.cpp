#include "Mesh.h"
#include "../Geometry/BoundingBox.h"

void InstanceData::moveFrom(MeshData& md)
{
	if (md.v3.size())
	{
		setVertices(md.v3, md.vertexMode, md.vertexLocation);
	} 
	else if (md.v4.size())
	{
		setVertices(md.v4, md.vertexMode, md.vertexLocation);
	}
	else
	{
		throw NMSLogicException("InstanceData::moveFrom() md must contain exactly a single mesh. Cannot recover.");
	}
	vertexLocation = md.vertexLocation;
	vertexMode = md.vertexMode;
}


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
		throw NMSLogicException("No data for InstanceData.");
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
		throw NMSLogicException("No data for MeshData.");
	}
}
