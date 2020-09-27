#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>
#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "VertexSource.h"
#include "../Helpers/ResizeHelper.h"

/*
	class to render class VertexSource.
	Hides all the vao/vbo/ebo and texture functionaility.
	Accepts callbacks to assist renderering
*/
class ResizeHelper;

class OWENGINE_API VertexRenderer
{
public:
	VertexRenderer();
	void addResizer(ResizeHelper* resizer) { mResizer = resizer; }
	void addSource(const VertexSource* rs) 
	{ 
		mSource = rs;
		prepareOpenGL();
	}
	void render(const glm::mat4& proj,
			const glm::mat4& view,
			const glm::mat4& model,
			VertexSource::SourceCallbackType renderCb = nullptr,
			ResizeHelper::ResizeCallbackType resizeCb = nullptr) const;
protected:
	const VertexSource* mSource;
private:
	ResizeHelper* mResizer = 0;
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
	void prepareOpenGL();
	void checkForErrors();
};
