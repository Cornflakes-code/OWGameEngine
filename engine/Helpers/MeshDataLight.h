#pragma once
#include <vector>
#include <string>
#include <limits.h>

#include <glm/glm.hpp>

class MeshDataLight
{
public:
	void vertices(const std::vector<glm::vec3>& v, 
		unsigned int vertexMode, unsigned int vertexLocation = 0)
	{
		mVec4.clear();
		mVec3 = v;
		mRenderData.verticesCount = v.size();
		mRenderData.vertexMode = vertexMode;
		mRenderData.vertexLocation = vertexLocation;
	}
	void vertices(const std::vector<glm::vec4>& v,
		unsigned int vertexMode, unsigned int vertexLocation = 0)
	{
		mVec3.clear();
		mVec4 = v;
		mRenderData.verticesCount = v.size();
		mRenderData.vertexMode = vertexMode;
		mRenderData.vertexLocation = vertexLocation;
	}
	void colour(const glm::vec4& colour, const std::string& shaderColourName)
	{
		mRenderData.colour = colour;
		mRenderData.shaderColourName = shaderColourName;
	}
	void indices(const std::vector<unsigned int> _indices, unsigned int indicesMode)
	{
		mIndices = _indices;
		mRenderData.indicesMode = indicesMode;
		mRenderData.indicesCount = _indices.size();
	}
private:
	struct RenderData
	{
		std::string shaderColourName;
		glm::vec4 colour = {0, 0, 0, 0};

		size_t verticesCount = 0;
		size_t indicesCount = 0;
		unsigned int vertexMode = UINT_MAX;
		unsigned int vertexLocation = UINT_MAX;
		unsigned int indicesMode = UINT_MAX;
	};
	std::vector<glm::vec3> mVec3;
	std::vector<glm::vec4> mVec4;
	std::vector<unsigned int> mIndices;
	RenderData mRenderData;
	friend class LightRenderer;
};
