#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>
#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "VertexSourceCompound.h"

/*
	class to render class VertexSource.
	Hides all the vao/vbo/ebo and texture functionaility.
	Accepts callbacks to assist renderering
*/

class OWENGINE_API VertexRendererCompound
{
public:
	VertexRendererCompound();
	~VertexRendererCompound();
	void add(VertexSourceCompound* source) { mSource = source; }
	void render(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& model) const
	{
		mSource->render(proj, view, model);
	}

protected:
	VertexSourceCompound* mSource;
private:
};
