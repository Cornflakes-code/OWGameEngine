#pragma once

#include <glm/glm.hpp>

#include "../Renderables/Renderer.h"
#include "../Renderables/SimpleVertexSource.h"

/*
	Simple class wrapping creation and Rendering of a Triangle. Will be removed.
*/
class Shader;

class Triangle: public SimpleVertexSource
{
	glm::vec3 mPosition = glm::vec3(0);
	std::vector<glm::vec4> mVertices;
	std::vector<unsigned int> mIndices;
	Shader* mShader;
public:
	Triangle();
	void setUp();
	void setPosition(const glm::vec3& newValue);
	void move(const glm::vec3& newValue);

	GLuint texture() const override { return 0; }
	const Shader* shader() const override { return mShader; }
	std::vector<glm::vec4> vertices(std::string& verticeLocation) const override
	{
		verticeLocation = "aPos";
		return mVertices;
	}
	std::vector<unsigned int> indices(unsigned int& type) const override
	{
		type = GL_TRIANGLES;
		return mIndices;
	}

	void doRender(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model) const override;
};