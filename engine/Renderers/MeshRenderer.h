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
	OWMeshRenderer(const std::string& shaderName, const std::vector<GPUBufferObject::BufferType>& orderedTypes,
		GPUBufferObject::BufferStyle bs)// = GPUBufferObject::BufferStyle::SSBO)
	: OWRenderer(shaderName, orderedTypes)
	{
		mSSBO.bufferStyle(bs);
	}
	OWMeshRenderer(Shader* shader, const std::vector<GPUBufferObject::BufferType>& orderedTypes,
		GPUBufferObject::BufferStyle bs)// = GPUBufferObject::BufferStyle::SSBO)
		: OWRenderer(shader, orderedTypes)
	{
		mSSBO.bufferStyle(bs);
	}
	void doSetup(const OWRenderData& renderData) override;
protected:
	void add(const MeshData& meshData);
	void add(const Texture& texture);
	void doRender() override;
#pragma warning( push )
#pragma warning( disable : 4251 )

private:
	MeshData mData;
	Texture mTexture;
	size_t mPositionCount = 0;
	size_t mColourCount = 0;
	std::vector<unsigned int> mPrimitiveIndices;
	static unsigned int mPrimitiveRestart;

	// Draw multi vars
	std::vector<GLsizei> mMultiArrayStartIndexes;
	std::vector<GLsizei> mMultiArrayVertexCount;
	std::vector<void*> mMultiElementStartIndexes;
	std::vector<GLsizei> mMultiElementIndicesCounts;
	GLsizei mNumMeshes = 0;
	// mVbo[0] The VBO containing the triangles to draw
	// mVbo[1] The VBO containing the positions of the particles
	// mVbo[2] The VBO containing the colors of the particles
	unsigned int mVao = std::numeric_limits<unsigned int>::max();
	//unsigned int mVbo = std::numeric_limits<unsigned int>::max();
	unsigned int mVbo[10] = { 0, 0, 0 };
	unsigned int mEbo = std::numeric_limits<unsigned int>::max();
	unsigned int mPrimitiveEbo = std::numeric_limits<unsigned int>::max();
	unsigned int mSbo = std::numeric_limits<unsigned int>::max();
#pragma warning( pop )
};
