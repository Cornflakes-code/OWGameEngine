#pragma once

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "RenderBase.h"

class Mesh;

class OWENGINE_API MeshRenderer : public RenderBase
{
public:
	MeshRenderer();
	~MeshRenderer();

	void prepare(const Mesh* source);
	virtual void checkSourceForErrors(const Mesh* source) const;
	void render(const Mesh* source,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const;
protected:
private:
	unsigned int mVao = 0;
	unsigned int mVbo[2] = {0, 0};
	unsigned int mEbo = 0;
};
