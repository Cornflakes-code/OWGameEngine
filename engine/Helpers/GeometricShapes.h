#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API GeometricShapes
{
public:
	static std::vector<glm::vec2> circle(float radius, float arcRadians);
	static std::vector<glm::vec2> torus(float innerRadius, 
									float outerRadius, 
									float arcRadians);
	static std::vector<glm::vec3> rectangle(const glm::vec2& dims,
		const glm::vec2& bottomLeft = { 0.0f, 0.0f });
	static std::vector<glm::vec3> goldenRectangle(float scale,
		const glm::vec2& bottomLeft = { 0.0f, 0.0f });
	static std::vector<glm::vec3> pointsOnSphere(
		int numHoroSegments, int numVertSegments, float TAU);

	static std::vector<glm::vec3> fibonacciSphere(int numPoints, float TAU);
	static std::vector<glm::vec3> cube(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::vec3& scale = glm::vec3(100, 100, 100));
	static std::vector<glm::vec3> star(
		float innerRadius, float outerRadius,
		unsigned int numPoints);
};