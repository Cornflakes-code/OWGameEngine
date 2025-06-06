#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
class OWENGINE_API OWGeometricShapes
{
public:
	static std::pair<glm::vec3, glm::vec3> minMaxBox;

	// A circle with radius of 1.0 and two_pi/arcRadians lines drawing the circle
	static std::vector<glm::vec3> circle(float arcRadians = 0.1);
	static std::vector<glm::vec3> torus(float innerRadius, 
									float outerRadius, 
									float arcRadians);
	static std::vector<glm::vec3> rectangle(const glm::vec2& dims);
	static std::vector<glm::vec3> goldenRectangle();
	static std::vector<glm::vec3> pointsOnSphere(
		int numHoroSegments, int numVertSegments, float TAU);

	static std::vector<glm::vec3> fibonacciSphere(int numPoints, float TAU);
	static std::vector<glm::vec3> cube();
	static std::vector<glm::vec3> tetrahedron();
	static std::vector<glm::vec3> line(float thickness = 1);
	// return a beam from beamEnd/2.0 to -beamEnd/2.0
	static std::vector<glm::vec3> beam(const glm::vec3& beamEnd);
	static std::vector<glm::vec3> beam(const glm::vec3& direction, float length);
	static std::vector<glm::vec3> star(
		float innerRadius, float outerRadius,
		unsigned int numPoints);
};