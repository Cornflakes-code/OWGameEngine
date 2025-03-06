#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "../Helpers/Mesh.h"
#include "RenderTypes.h"
#include "RendererBase.h"

class OWMeshRenderer: public OWRenderer
{
public:
	enum RenderType { DRAW_NONE, DRAW_ARRAYS, DRAW_MULTI, DRAW_PRIMITIVE };

	OWMeshRenderer(const std::string& shaderName, RenderType rt = DRAW_ARRAYS)
	: OWRenderer(shaderName), mDrawType(rt)
	{
	}
	OWMeshRenderer(Shader* shader, RenderType rt = DRAW_ARRAYS)
		: OWRenderer(shader), mDrawType(rt)
	{
	}
	void doSetup(const std::vector<OWMeshData>& meshes,
		const std::vector<OWModelData>& models) override;
protected:
	void add(const MeshData& meshData);
	void doRender() override;
	void prepareOpenGL();
#pragma warning( push )
#pragma warning( disable : 4251 )
	struct RenderData
	{
		std::string shaderColourName;
		glm::vec4 colour = { 0, 0, 0, 0 };

		size_t verticesCount = 0;
		size_t indicesCount = 0;
		unsigned int vertexMode = UINT_MAX;
		unsigned int vertexLocation = UINT_MAX;
		unsigned int indicesMode = UINT_MAX;
		unsigned int mPolygonMode_mode = UINT_MAX;
	};
	std::vector<RenderData> mRenderData;
	std::vector<glm::vec3> mVec3;
	std::vector<glm::vec4> mVec4;
	std::vector<unsigned int> mIndices;

	std::vector<unsigned int> mPrimitiveIndices;
	static unsigned int mPrimitiveRestart;

	std::vector<GLint> mMultiArrayStartIndexes;
	std::vector<GLsizei> mMultiArrayVertexCount;
	// mVbo[0] The VBO containing the triangles to draw
	// mVbo[1] The VBO containing the positions of the particles
	// mVbo[2] The VBO containing the colors of the particles
	unsigned int mVao = std::numeric_limits<unsigned int>::max();
	unsigned int mVbo = std::numeric_limits<unsigned int>::max();
	unsigned int mEbo = std::numeric_limits<unsigned int>::max();
	unsigned int mPrimitiveEbo = std::numeric_limits<unsigned int>::max();
	RenderType mDrawType = DRAW_NONE;
#pragma warning( pop )
};
