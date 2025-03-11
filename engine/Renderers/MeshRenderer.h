#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "../Helpers/Mesh.h"
#include "RenderTypes.h"
#include "RendererBase.h"

class OWENGINE_API OWMeshRenderer: public OWRenderer
{
public:
	OWMeshRenderer(const std::string& shaderName, RenderType rt = DRAW_MULTI)
	: OWRenderer(shaderName, rt)
	{
	}
	OWMeshRenderer(Shader* shader, RenderType rt = DRAW_MULTI)
		: OWRenderer(shader, rt)
	{
	}
	void doSetup(const OWRenderData& renderData) override;
protected:
	void add(const MeshData& meshData);
	void add(const Texture& texture);
	void doRender() override;
	void prepareOpenGL();
#pragma warning( push )
#pragma warning( disable : 4251 )


	MeshData mData;
	Texture mTexture;
	size_t mPositionCount = 0;
	size_t mColourCount = 0;
	std::vector<unsigned int> mPrimitiveIndices;
	static unsigned int mPrimitiveRestart;

	// Draw multi vars
	std::vector<GLsizei> mMultiArrayStartIndexes;
	std::vector<GLsizei> mMultiArrayVertexCount;
	std::vector<GLsizei> mMultiElementStartIndexes;
	std::vector<GLsizei> mMultiElementIndicesCounts;
	GLsizei mNumMeshes = 0;
	// mVbo[0] The VBO containing the triangles to draw
	// mVbo[1] The VBO containing the positions of the particles
	// mVbo[2] The VBO containing the colors of the particles
	unsigned int mVao = std::numeric_limits<unsigned int>::max();
	unsigned int mVbo = std::numeric_limits<unsigned int>::max();
	unsigned int mEbo = std::numeric_limits<unsigned int>::max();
	unsigned int mPrimitiveEbo = std::numeric_limits<unsigned int>::max();
	unsigned int mSbo = std::numeric_limits<unsigned int>::max();
#pragma warning( pop )
};
