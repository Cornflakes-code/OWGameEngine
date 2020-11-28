#pragma once

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "VertexRenderBase.h"

class Model;

class OWENGINE_API ModelRenderer : public VertexRenderBase
{
public:
	ModelRenderer();
	~ModelRenderer();
	void prepare(const Model* source);
	void render(const Model* vsb,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const;
protected:
private:
	virtual void checkSourceForErrors(const Model* source);
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
};
