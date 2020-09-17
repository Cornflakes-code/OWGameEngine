#pragma once

#include <glm/glm.hpp>

#include "../Renderables/Renderer.h"
#include "../Renderables/SimpleVertexSource.h"

class Shader;
/*
	Simple class wrapping creation and Rendering of a Pyramid. 
	Used for initial testing. Will be removed.
*/
class Pyramid: public SimpleVertexSource
{
	glm::vec3 mPosition = glm::vec3(0);
	std::vector<glm::vec4> mVertices;
	std::vector<unsigned int> mIndices;
	Shader* mShader;
public:
	Pyramid();
	void setUp();
	void setPosition(const glm::vec3& newValue);
	void move(const glm::vec3& newValue);

	GLuint texture() const override { return 0; }
	const Shader* shader() const override { return mShader; }
	std::vector<glm::vec4> vertices(std::string& verticeLocation,
									unsigned int& type) const override
	{
		verticeLocation = "aPos";
		type = GL_TRIANGLES;
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