#pragma once

#include <glm/glm.hpp>

#include "../Renderables/Renderer.h"

class Shader;

class Triangle: public Renderer
{
	glm::vec3 mPosition = glm::vec3(0);
public:
	Triangle(Shader* _shader = nullptr);
	void setUp();
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model) const override;

	void setPosition(const glm::vec3& newValue);
	void move(const glm::vec3& newValue);
};