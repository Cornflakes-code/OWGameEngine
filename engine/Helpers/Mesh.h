#pragma once
#include <vector>
#include <string>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "../Geometry/BoundingBox.h"

struct OWENGINE_API MeshData
{
	MeshData& addVertices(const std::vector<glm::vec3>& v);
	MeshData& addVertices(const std::vector<glm::vec4>& v);
	MeshData& addIndices(const std::vector<unsigned int> v);
	MeshData& setColour(const glm::vec4& _colour, const std::string& _shaderColourName);
	MeshData& setModes(unsigned int _vertexMode = GL_TRIANGLES,
			unsigned int _indicesMode = GL_TRIANGLES,
			unsigned int _polygonMode = GL_FILL);
	// Can be GL_POINT, GL_LINE or GL_FILL
	MeshData& setPolygonMode(unsigned int _mode);

	std::string shaderColourName;
	glm::vec4 colour = { 0, 0, 0, 0 };

	std::vector<glm::vec4> v4;
	std::vector<unsigned int> indices;
	unsigned int vertexMode = UINT_MAX;
	unsigned int indicesMode = UINT_MAX;
	unsigned int polygonMode_mode = UINT_MAX;
	AABB bounds() const;
	bool updateMeshOnly() const {
		return mUpdateMeshOnly;
	}
	void updateMeshOnly(bool newValue) {
		mUpdateMeshOnly = newValue;
	}
private:
	bool mUpdateMeshOnly = false;
};

