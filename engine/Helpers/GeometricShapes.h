#pragma once

#include <vector>

#include <glm/glm.hpp>

class GeometricShapes
{
public:
	GeometricShapes();
	static std::vector<glm::vec2> circle(float radius, float arcRadians);
	static std::vector<glm::vec2> torus(float innerRadius, 
									float outerRadius, 
									float arcRadians);
	static std::vector<glm::vec2> rectangle(const glm::vec2& dims, 
						const glm::vec2& bottomLeft = { 0.0f, 0.0f });
};