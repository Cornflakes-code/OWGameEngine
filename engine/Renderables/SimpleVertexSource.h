#pragma once

#include <vector>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

class Shader;

class SimpleVertexSource
{
public:
	SimpleVertexSource();
	virtual void doRender(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model) const = 0;
	virtual GLuint texture() const = 0;
	virtual const Shader* shader() const = 0;
	virtual std::string verticeLocation() const = 0;
	virtual const std::vector<glm::vec4>& vertices() const = 0;
};
