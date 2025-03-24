#pragma once
#include <vector>
#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "../Helpers/Model.h"
#include "RenderTypes.h"
#include "RendererBase.h"

class OWENGINE_API OWModelRenderer: public OWRenderer
{
public:
	OWModelRenderer(const std::string& shaderName, const std::vector<GPUBufferObject::BufferType>& orderedTypes,
		GPUBufferObject::BufferStyle bs)// = GPUBufferObject::BufferStyle::SSBO)
		: OWRenderer(shaderName, orderedTypes)
	{
		mSSBO.bufferStyle(bs);
	}
	OWModelRenderer(Shader* shader, const std::vector<GPUBufferObject::BufferType>& orderedTypes,
		GPUBufferObject::BufferStyle bs)// = GPUBufferObject::BufferStyle::SSBO)
		: OWRenderer(shader, orderedTypes)
	{
		mSSBO.bufferStyle(bs);
	}
	void doSetup(const OWRenderData& renderData) override;
protected:
	void doRender() override;
	void add(const Texture& texture);
	void add(const OWModelData& md);
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	OWModelData mData;
	// Draw multi vars
	std::vector<GLsizei> mMultiArrayStartIndexes;
	std::vector<GLsizei> mMultiArrayVertexCount;
	std::vector<void*> mMultiElementStartIndexes;
	std::vector<GLsizei> mMultiElementIndicesCounts;

	unsigned int mVao = 0;
	// mVbo[0] The VBO containing the vertexes
	// Cannot use more VBP's than what there are Buffer Objects (plus one for the vertexes)
	unsigned int mVbo[static_cast<int>(GPUBufferObject::BufferType::NoType) + 2] = { std::numeric_limits<unsigned int>::max() };
	unsigned int mSbo = std::numeric_limits<unsigned int>::max();
	unsigned int mEbo = 0;
#pragma warning( pop )
	void continueSetup();
};
