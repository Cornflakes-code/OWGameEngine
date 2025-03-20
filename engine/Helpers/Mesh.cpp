#include "Mesh.h"
#include "../Geometry/BoundingBox.h"
#include <glm/gtc/type_ptr.hpp>

MeshData& MeshData::addVertices(const std::vector<glm::vec3>& v)
{
	for (const glm::vec3& p3 : v)
	{
		const glm::vec4& p4 = glm::vec4(p3, 0);
		v4.push_back(p4);
	}
	return *this;
}

MeshData& MeshData::addVertices(const std::vector<glm::vec4>& v)
{
	v4.insert(v4.begin(), v.begin(), v.end());
	return *this;
}

MeshData& MeshData::addIndices(const std::vector<unsigned int> v)
{
	indices.insert(indices.begin(), v.begin(), v.end());
	return *this;
}

MeshData& MeshData::setColour(const glm::vec4& _colour, const std::string& _shaderColourName)
{
	colour = _colour;
	shaderColourName = _shaderColourName;
	return *this;
}

MeshData& MeshData::setModes(unsigned int _vertexMode,
	unsigned int _indicesMode, unsigned int _polygonMode)
{
	vertexMode = _vertexMode;
	indicesMode = _indicesMode;
	polygonMode_mode = _polygonMode;
	return *this;
}

// Can be GL_POINT, GL_LINE or GL_FILL
MeshData& MeshData::setPolygonMode(unsigned int _mode)
{
	polygonMode_mode = _mode;
	return *this;
}

AABB MeshData::bounds() const
{
	if (v4.size())
	{
		return AABB(v4);
	}
	else
	{
		throw NMSLogicException("No data for MeshData.");
	}
}
