#pragma once

#include <vector>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

class Shader;
/*
	class containing basic vertex info info that gets passed to SimpleVertexRender
*/
class SimpleVertexSource
{
public:
	SimpleVertexSource();
	virtual void doRender(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model) const = 0;
	virtual GLuint texture() const = 0;
	virtual const Shader* shader() const = 0;
	virtual std::vector<glm::vec4> vertices(std::string& vertexLocation) const = 0;
	virtual std::vector<unsigned int> indices(unsigned int& type) const = 0;
};
