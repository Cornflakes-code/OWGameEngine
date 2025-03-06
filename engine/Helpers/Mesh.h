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
	void vertices(const std::vector<glm::vec3>& v,
		unsigned int vertexMode, unsigned int vertexLocation = 0)
	{
		v4.clear();
		v3 = v;
		renderData.verticesCount = v.size();
		renderData.vertexMode = vertexMode;
		renderData.vertexLocation = vertexLocation;
	}
	void vertices(const std::vector<glm::vec4>& v,
		unsigned int vertexMode, unsigned int vertexLocation = 0)
	{
		v3.clear();
		v4 = v;
		renderData.verticesCount = v.size();
		renderData.vertexMode = vertexMode;
		renderData.vertexLocation = vertexLocation;
	}
	void positions(const std::vector<glm::vec3>& positions,
		unsigned int positionDivisor,
		unsigned int positionLocation)
	{
		instancePositions = positions;
		renderData.positionCount = positions.size();
		renderData.positionDivisor = positionDivisor;
		renderData.positionLocation = positionLocation;
	}
	void colours(const std::vector<glm::vec4>& colours,
		unsigned int colourDivisor,
		unsigned int colourLocation)
	{
		instanceColours = colours;
		renderData.colourDivisor = colourDivisor;
		renderData.colourLocation = colourLocation;
	}
	struct RenderData
	{
		size_t verticesCount = UINT_MAX;
		unsigned int vertexMode = UINT_MAX;
		unsigned int vertexLocation = UINT_MAX;

		// Instancing Data
		unsigned int positionDivisor = UINT_MAX;
		unsigned int positionLocation = UINT_MAX;
		size_t positionCount = UINT_MAX;
		unsigned int colourDivisor = UINT_MAX;
		unsigned int colourLocation = UINT_MAX;
	};
	std::vector<glm::vec3> v3;
	std::vector<glm::vec4> v4;
	std::vector<glm::vec3> instancePositions;
	std::vector<glm::vec4> instanceColours;
	RenderData renderData;
	AABB bounds() const;
};

struct MeshData
{
	void setVertices(const std::vector<glm::vec3>& v,
		unsigned int vertexMode, unsigned int vertexLocation = 0)
	{
		v4.clear();
		v3 = v;
		renderData.verticesCount = v.size();
		renderData.vertexMode = vertexMode;
		renderData.vertexLocation = vertexLocation;
	}
	void setVertices(const std::vector<glm::vec4>& v,
		unsigned int vertexMode, unsigned int vertexLocation = 0)
	{
		v3.clear();
		v4 = v;
		renderData.verticesCount = v.size();
		renderData.vertexMode = vertexMode;
		renderData.vertexLocation = vertexLocation;
	}
	void setColour(const glm::vec4& colour, const std::string& shaderColourName)
	{
		renderData.colour = colour;
		renderData.shaderColourName = shaderColourName;
	}
	void setIndices(const std::vector<unsigned int> _indices, unsigned int indicesMode)
	{
		indices = _indices;
		renderData.indicesMode = indicesMode;
		renderData.indicesCount = _indices.size();
	}
	// Can be GL_POINT, GL_LINE or GL_FILL
	void setPolygonMode(unsigned int mode)
	{
		renderData.polygonMode_mode = mode;
	}
	struct RenderData
	{
		std::string shaderColourName;
		glm::vec4 colour = { 0, 0, 0, 0 };

		size_t verticesCount = 0;
		size_t indicesCount = 0;
		unsigned int vertexMode = UINT_MAX;
		unsigned int vertexLocation = UINT_MAX;
		unsigned int indicesMode = UINT_MAX;
		unsigned int polygonMode_mode = UINT_MAX;
	};
	std::vector<glm::vec3> v3;
	std::vector<glm::vec4> v4;
	std::vector<unsigned int> indices;
	RenderData renderData;
	AABB bounds() const;
};

struct OWMeshData
{
	InstanceData instanceData;
	MeshData meshData;
	AABB bounds() const 
	{
		return instanceData.bounds() | meshData.bounds();
	}
};
