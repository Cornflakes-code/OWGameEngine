#pragma once

#include <glm/glm.hpp>

#include "../Renderables/Renderer.h"

class Shader;

class Points: public Renderer
{
	glm::vec3 mPosition = glm::vec3(0);
public:
	Points(Shader* _shader = nullptr);
	void setUp(const std::vector<glm::vec3>& vertices);
	void render(const glm::mat4& proj, 
				const glm::mat4& view,
				const glm::mat4& model) const override;
	void setPosition(const glm::vec3& newValue);
	void move(const glm::vec3& newValue);
};