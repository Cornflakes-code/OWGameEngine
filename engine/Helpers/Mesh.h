#pragma once
#include <vector>
#include <string>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "../Geometry/BoundingBox.h"

struct InstanceData
{
	void setVertices(const std::vector<glm::vec3>& v,
		unsigned int _vertexMode, unsigned int _vertexLocation = 0)
	{
		v4.clear();
		v3 = v;
		vertexMode = _vertexMode;
		vertexLocation = _vertexLocation;
	}
	void setVertices(const std::vector<glm::vec4>& v,
		unsigned int _vertexMode, unsigned int _vertexLocation = 0)
	{
		v3.clear();
		v4 = v;
		vertexMode = _vertexMode;
		vertexLocation = _vertexLocation;
	}
	void setPositions(const std::vector<glm::vec3>& _positions,
		unsigned int _positionDivisor,
		unsigned int _positionLocation)
	{
		instancePositions = _positions;
		positionDivisor = _positionDivisor;
		positionLocation = _positionLocation;
	}
	void setColours(const std::vector<glm::vec4>& _colours,
		unsigned int _colourDivisor,
		unsigned int _colourLocation)
	{
		instanceColours = _colours;
		colourDivisor = _colourDivisor;
		colourLocation = _colourLocation;
	}
	unsigned int vertexMode = UINT_MAX;
	unsigned int vertexLocation = UINT_MAX;

	// Instancing Data
	unsigned int positionDivisor = UINT_MAX;
	unsigned int positionLocation = UINT_MAX;
	unsigned int colourDivisor = UINT_MAX;
	unsigned int colourLocation = UINT_MAX;
	std::vector<glm::vec3> v3;
	std::vector<glm::vec4> v4;
	std::vector<glm::vec3> instancePositions;
	std::vector<glm::vec4> instanceColours;
	AABB bounds() const;
};

struct MeshData
{
	void setVertices(const std::vector<glm::vec3>& v,
		unsigned int _vertexMode, unsigned int _vertexLocation = 0)
	{
		v4.clear();
		v3 = v;
		vertexMode = _vertexMode;
		vertexLocation = _vertexLocation;
	}
	void setVertices(const std::vector<glm::vec4>& v,
		unsigned int _vertexMode, unsigned int _vertexLocation = 0)
	{
		v3.clear();
		v4 = v;
		vertexMode = _vertexMode;
		vertexLocation = _vertexLocation;
	}
	void setColour(const glm::vec4& _colour, const std::string& _shaderColourName)
	{
		colour = _colour;
		shaderColourName = _shaderColourName;
	}
	// Can be GL_POINT, GL_LINE or GL_FILL
	void setPolygonMode(unsigned int _mode)
	{
		polygonMode_mode = _mode;
	}
	void setIndices(const std::vector<unsigned int> _indices, unsigned int _indicesMode)
	{
		indices = _indices;
		indicesMode = _indicesMode;
	}
	std::vector<glm::vec3> v3;
	std::vector<glm::vec4> v4;
	std::vector<unsigned int> indices;
	std::string shaderColourName;
	glm::vec4 colour = { 0, 0, 0, 0 };
	unsigned int vertexMode = UINT_MAX;
	unsigned int vertexLocation = UINT_MAX;
	unsigned int indicesMode = UINT_MAX;
	unsigned int polygonMode_mode = UINT_MAX;
	AABB bounds() const;
};

