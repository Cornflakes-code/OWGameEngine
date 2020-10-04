#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>
#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "VertexRenderBase.h"
/*
	class to render class VertexSource.
	Hides all the vao/vbo/ebo and texture functionaility.
	Accepts callbacks to assist renderering
*/

class VertexSource;

class OWENGINE_API VertexSourceRenderer: public VertexRenderBase
{
public:
	VertexSourceRenderer();
	~VertexSourceRenderer();
	void prepare(const VertexSource* source);
	void render(const VertexSource* vsb,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const;
protected:
private:
	virtual void checkSourceForErrors(const VertexSource* source);
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale) const;
	float aspectRatio() const;
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
	// Ok to modify Renderables if only for efficiency reasons
	mutable bool mFirstTimeRender = true;
};
