#pragma once

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "RenderBase.h"

class Mesh;

class OWENGINE_API ModelRenderer : public RenderBase
{
public:
	ModelRenderer();
	~ModelRenderer();

	void prepare(const Mesh* source);

	void render(const Mesh* source,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const;
protected:
private:
	virtual void checkSourceForErrors(const Mesh* source);
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
};
