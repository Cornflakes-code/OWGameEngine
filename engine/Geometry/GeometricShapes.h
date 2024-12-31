#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API GeometricShapes
{
public:
	static std::pair<glm::vec3, glm::vec3> minMaxBox;
	static std::vector<glm::vec3> circle(float radius, float arcRadians);
	static std::vector<glm::vec3> torus(float innerRadius, 
									float outerRadius, 
									float arcRadians);
	static std::vector<glm::vec3> rectangle(const glm::vec2& dims);
	static std::vector<glm::vec3> goldenRectangle(float scale,
		const glm::vec2& bottomLeft = { 0.0f, 0.0f });
	static std::vector<glm::vec3> pointsOnSphere(
		int numHoroSegments, int numVertSegments, float TAU);

	static std::vector<glm::vec3> fibonacciSphere(int numPoints, float TAU);
	static std::vector<glm::vec3> cube(const glm::vec3& scale = glm::vec3(1, 1, 1));
	static std::vector<glm::vec3> star(
		float innerRadius, float outerRadius,
		unsigned int numPoints);
};