#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

#ifndef __gl_h_
#include <glad/glad.h>
#endif

class SimpleVertexSource;

class OWENGINE_API SimpleVertexRender
{
	const SimpleVertexSource* mSource = nullptr;
	GLuint mVao = 0;
	GLuint mVbo = 0;
	GLuint mEbo = 0;
public:
	SimpleVertexRender() {}
	void setUp(SimpleVertexSource* _source);
	void render(const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model) const;
};
