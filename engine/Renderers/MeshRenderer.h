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

	// Draw multi vars
	std::vector<GLsizei> mMultiArrayStartIndexes;
	std::vector<GLsizei> mMultiArrayVertexCount;
	std::vector<void*> mMultiElementStartIndexes;
	std::vector<GLsizei> mMultiElementIndicesCounts;
	// mVbo[1] The VBO containing the positions of the particles
	// mVbo[2] The VBO containing the colors of the particles
	unsigned int mVao = std::numeric_limits<unsigned int>::max();

	// mVbo[0] The VBO containing the vertexes
	// Cannot use more VBP's than what there are Buffer Objects (plus one for the vertexes)
	unsigned int mVbo[static_cast<int>(GPUBufferObject::BufferType::NoType) + 2] = { std::numeric_limits<unsigned int>::max() };
	unsigned int mEbo = std::numeric_limits<unsigned int>::max();
	unsigned int mPrimitiveEbo = std::numeric_limits<unsigned int>::max();
	unsigned int mSbo = std::numeric_limits<unsigned int>::max();
#pragma warning( pop )
	void continueSetup();
};
