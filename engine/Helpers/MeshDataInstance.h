#pragma once
#include <vector>
#include <string>

#include <glm/glm.hpp>

class MeshDataInstance
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
	void positions(const std::vector<glm::vec3>& positions,
		unsigned int positionDivisor,
		unsigned int positionLocation)
	{
		mInstancePositions = positions;
		mRenderData.positionCount = positions.size();
		mRenderData.positionDivisor = positionDivisor;
		mRenderData.positionLocation = positionLocation;
	}
	void colours(const std::vector<glm::vec4>& colours,
		unsigned int colourDivisor,
		unsigned int colourLocation)
	{
		mInstanceColours = colours;
		mRenderData.colourDivisor = colourDivisor;
		mRenderData.colourLocation = colourLocation;
	}
private:
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
	std::vector<glm::vec3> mVec3;
	std::vector<glm::vec4> mVec4;
	std::vector<glm::vec3> mInstancePositions;
	std::vector<glm::vec4> mInstanceColours;
	RenderData mRenderData;
	friend class InstanceRenderer;
};
