#include "Component.h"
#include "../Core/ErrorHandling.h"
/*
const std::vector<MeshData> OWMeshComponentBase::meshData(AABB& bounds) const
{
	bounds = AABB(0);
	return std::vector<MeshData>();
}

const std::vector<InstanceData> OWMeshComponentBase::instanceData(AABB& bounds) const
{
	bounds = AABB(0);
	return std::vector<InstanceData>();
}

const std::vector<OWModelData> OWMeshComponentBase::modelData(AABB& bounds) const
{
	bounds = AABB(0);
	return std::vector<OWModelData>();
}
*/

void OWComponent::name(const std::string& _name)
{
	if (mName.size() != 0)
		throw NMSLogicException(std::stringstream()
			<< "OWComponent ["
			<< mName << "] is incorrectly trying to be renamed to ["
			<< _name << "[\n");
	mName = _name;
}

bool OWMeshComponentBase::validLocation(int location)
{
	return (location > -1) && (location < 1000);
}

bool OWMeshComponentBase::validMode(GLenum mode)
{
	return (mode == GL_POINTS) ||
		(mode == GL_LINE_STRIP) ||
		(mode == GL_LINE_LOOP) ||
		(mode == GL_LINES) ||
		(mode == GL_LINE_STRIP_ADJACENCY) ||
		(mode == GL_LINES_ADJACENCY) ||
		(mode == GL_TRIANGLE_STRIP) ||
		(mode == GL_TRIANGLE_FAN) ||
		(mode == GL_TRIANGLES) ||
		(mode == GL_TRIANGLE_STRIP_ADJACENCY) ||
		(mode == GL_TRIANGLES_ADJACENCY) ||
		(mode == GL_PATCHES);
}

bool OWMeshComponentBase::validPolygonMode(GLenum mode)
{
	return (mode == GL_POINT) ||
		(mode == GL_LINE) ||
		(mode == GL_FILL);
}

void OWMeshComponentBase::validate(const MeshData& md) const
{
	if (md.v4.empty())
	{
		if (md.v4.empty())
			throw NMSLogicException("OWMeshComponentBase::validate [" 
				+ name() + "] MeshData has no data\n");
		else
			throw NMSLogicException("OWMeshComponentBase::validate [" \
				+ name() + "] MeshData has both v3 and v4 data\n");
	}
	if (md.indices.size())
	{
		if (!validMode(md.indicesMode))
			throw NMSLogicException("OWMeshComponentBase::validate [" 
				+ name()
				+ "] MeshData invalid indicesMode [" + std::to_string(md.indicesMode) + "]\n");
	}
	if (!validMode(md.vertexMode))
		throw NMSLogicException("OWMeshComponentBase::validate [" + name()
			+ "] MeshData invalid vertexMode [" + std::to_string(md.vertexMode) + "]\n");
	if (!validPolygonMode(md.polygonMode_mode))
		throw NMSLogicException("OWMeshComponentBase::validate [" + name()
			+ "] MeshData invalid polygon mode mode [" + std::to_string(md.polygonMode_mode) + "]\n");
}

void OWMeshComponentBase::validate(const OWModelData& md) const
{
	throw NMSLogicException("OWMeshComponentBase::validate model [" + name()
		+ "] not implemented \n");
}

void OWMeshComponentBase::validate(const GPUBufferObject& ss) const
{
	ss.validate(name());
}
