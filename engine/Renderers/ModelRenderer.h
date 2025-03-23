#pragma once
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "RendererBase.h"
#include "../Helpers/Model.h"

class OWENGINE_API OWModelRenderer: public OWRenderer
{
public:
	void doSetup(const OWRenderData& renderData) override;
	OWModelRenderer(const std::string& shaderFileName, const std::vector<GPUBufferObject::BufferType>& orderedTypes)
		: OWRenderer(shaderFileName, orderedTypes)
	{
	}
	OWModelRenderer(Shader* shader, const std::vector<GPUBufferObject::BufferType>& orderedTypes)
		: OWRenderer(shader, orderedTypes)
	{
	}
protected:
	void doRender() override;
	void add(const Texture& texture);
	void add(const OWModelData& modelData);
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	OWModelData mData;
	unsigned int mVertexLocation = GL_INVALID_ENUM;
	unsigned int mVao = 0;
	// mVbo[0] The VBO containing the vertexes
	// Cannot use more VBP's than what there are Buffer Objects (plus one for the vertexes)
	unsigned int mVbo[static_cast<int>(GPUBufferObject::BufferType::NoType) + 2] = { std::numeric_limits<unsigned int>::max() };
	unsigned int mSbo = std::numeric_limits<unsigned int>::max();
	unsigned int mEbo = 0;
#pragma warning( pop )
	void continueSetup();
};
