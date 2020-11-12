#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API GeometricShapes
{
public:
	GeometricShapes();
	static std::vector<glm::vec2> circle(float radius, float arcRadians);
	static std::vector<glm::vec2> torus(float innerRadius, 
									float outerRadius, 
									float arcRadians);
	static std::vector<glm::vec3> rectangle(const glm::vec2& dims, 
						const glm::vec2& bottomLeft = { 0.0f, 0.0f });
	static std::vector<glm::vec3> pointsOnSphere(
		int numHoroSegments, int numVertSegments, float TAU);

	static std::vector<glm::vec3> fibonacciSphere(int numPoints, float TAU);

};