#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>
#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "VertexSource.h"

/*
	class to render class VertexSource.
	Hides all the vao/vbo/ebo and texture functionaility.
	Accepts callbacks to assist renderering
*/

class OWENGINE_API VertexRenderer
{
public:
	VertexRenderer();
	~VertexRenderer();
	void addSource(const VertexSource* rs) 
	{ 
		mSource = rs;
		checkSourceForErrors();
		prepareOpenGL();
	}
	void render(const glm::mat4& proj,
			const glm::mat4& view,
			const glm::mat4& model,
			VertexSource::RenderCallbackType renderCb = nullptr,
			VertexSource::ResizeCallbackType resizeCb = nullptr) const;
protected:
	const VertexSource* mSource;
private:
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale) const;
	float aspectRatio() const;
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
	void prepareOpenGL();
	void checkSourceForErrors();
};
