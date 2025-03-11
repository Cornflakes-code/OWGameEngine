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
	OWModelRenderer(const std::string& shaderFileName, RenderType rt = DRAW_MULTI)
		: OWRenderer(shaderFileName, rt)
	{
	}
	OWModelRenderer(Shader* shader, RenderType rt = DRAW_MULTI)
		: OWRenderer(shader, rt)
	{
	}
protected:
	void doRender() override;
	void prepareOpenGL();
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	OWModelData mData;
	unsigned int mVertexLocation = GL_INVALID_ENUM;
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
#pragma warning( pop )
};
