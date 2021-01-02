#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>
#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "VerticesRenderer.h"
/*
	class to render class SimpleModel.
	Hides all the vao/vbo/ebo and texture functionaility.
	Accepts callbacks to assist renderering
*/

class SimpleModel;

class OWENGINE_API SimpleModelRenderer: public VerticesRenderer
{
public:
	SimpleModelRenderer();
	~SimpleModelRenderer();
	void prepare(const SimpleModel* source);
	virtual void checkSourceForErrors(const SimpleModel* source) const;
	void render(const SimpleModel* vsb,
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		OWUtils::RenderCallbackType renderCb = nullptr,
		OWUtils::ResizeCallbackType resizeCb = nullptr) const;
protected:
private:
	unsigned int mVao = 0;
	unsigned int mVbo = 0;
	unsigned int mEbo = 0;
};
