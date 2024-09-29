#include "GeometricShapes.h"

#include <glm/gtc/constants.hpp>

std::vector<glm::vec2> GeometricShapes::circle(float radius, float arcRadians)
{
	std::vector<glm::vec2> retval;
	for (float i = 0.0f; i < glm::two_pi<float>(); i += arcRadians)
	{
		retval.push_back(glm::vec2(radius * glm::cos(i), radius * glm::sin(i)));
	}
	return retval;
}

std::vector<glm::vec2> GeometricShapes::torus(float innerRadius,
										float outerRadius,
										float arcRadians)
{
	std::vector<glm::vec2> retval;
	for (float i = 0.0f; i < glm::two_pi<float>(); i += arcRadians)
	{
		float arc = i + arcRadians;

		retval.push_back(glm::vec2(innerRadius * glm::cos(i), innerRadius * glm::sin(i)));
		retval.push_back(glm::vec2(outerRadius * glm::cos(i), outerRadius * glm::sin(i)));
		retval.push_back(glm::vec2(outerRadius * glm::cos(arc), outerRadius * glm::sin(arc)));

		retval.push_back(glm::vec2(innerRadius * glm::cos(i), innerRadius * glm::sin(i)));
		retval.push_back(glm::vec2(outerRadius * glm::cos(arc), outerRadius * glm::sin(arc)));
		retval.push_back(glm::vec2(innerRadius * glm::cos(arc), innerRadius * glm::sin(arc)));
	}
	return retval;
}

std::vector<glm::vec3> GeometricShapes::rectangle(const glm::vec2& dims, 
							const glm::vec2& bottomLeft)
{
	const float& x = bottomLeft.x;
	const float& y = bottomLeft.y;
	float z = 0.0f;
	std::vector<glm::vec3> retval;
	retval.push_back({ x, y, z });
	retval.push_back({ x, dims.y + y, z });
	retval.push_back({ dims.x + x, y, z });

	retval.push_back({ dims.x + x, y, z });
	retval.push_back({ x, dims.y + y, z });
	retval.push_back({ dims.x + x, dims.y + y, z });

	return retval;
}

// But the points are bunched near the poles.
std::vector<glm::vec3> GeometricShapes::pointsOnSphere(
	int numHoroSegments, int numVertSegments, float TAU)
{
	// https://www.youtube.com/watch?v=lctXaT9pxA0
	std::vector<glm::vec3> retVal;
	for (int h = 0; h < numHoroSegments; h++)
	{
		float angle1 = (h + 1) * glm::pi<float>() / (numHoroSegments + 1);
		for (int v = 0; v < numVertSegments; v++)
		{
			float angle2 = v * TAU / numVertSegments;
				float x = sin(angle1) * cos(angle2);
				float y = cos(angle1);
				float z = sin(angle1) * sin(angle2);
				glm::vec3 pointOnSphere = glm::vec3(x, y, z);
				retVal.push_back(pointOnSphere);
		}
	}
	return retVal;
}

// Evenly distributed points on a sphere.
std::vector<glm::vec3> GeometricShapes::fibonacciSphere(int numPoints, float TAU)
{
	// https://www.youtube.com/watch?v=lctXaT9pxA0

	// https://www.redblobgames.com/x/1842-delaunay-voronoi-sphere/
	std::vector<glm::vec3> retVal;

	const double goldenRatio = (1 + sqrt(5)) / 2.0;
	const double angleIncrement = TAU * goldenRatio;
	for (int i = 0; i < numPoints; i++)
	{
		double t = (i * 1.0) / numPoints;
		double angle1 = acos(1 - 2 * t);
		double angle2 = angleIncrement * i;
		float x = static_cast<float>(sin(angle1) * cos(angle2));
		float y = static_cast<float>(sin(angle1) * sin(angle2));
		float z = static_cast<float>(cos(angle1));
		glm::vec3 pointOnSphere = glm::vec3(x, y, z);
		retVal.push_back(pointOnSphere);
	}
	return retVal;
}

std::vector<glm::vec3> GeometricShapes::star(float innerRadius, float outerRadius,
								unsigned int numPoints)
{
	std::vector<glm::vec3> retval;
	const double incr = glm::two_pi<float>() / numPoints;
	double angle1 = 0;
	double angle2 = angle1 + incr / 2.0;
	double angle3 = angle2 + incr / 2.0;

	for (unsigned int i = 0; i < numPoints; i++)
	{
		// iterate counter clockwise to create points

		// Do the triangles that form the star points
		retval.push_back(glm::vec3(innerRadius * glm::sin(angle1), innerRadius * glm::cos(angle1), 0));
		retval.push_back(glm::vec3(outerRadius * glm::sin(angle2), outerRadius * glm::cos(angle2), 0));
		retval.push_back(glm::vec3(innerRadius * glm::sin(angle3), innerRadius * glm::cos(angle3), 0));
		// Do the triangles that form the body of the star
		retval.push_back(glm::vec3(innerRadius * glm::sin(angle3), innerRadius * glm::cos(angle3), 0));
		retval.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		retval.push_back(glm::vec3(innerRadius * glm::sin(angle1), innerRadius * glm::cos(angle1), 0));
		angle1 += incr;
		angle2 += incr;
		angle3 += incr;
	}
	return retval;
}

std::vector<glm::vec3> GeometricShapes::cube(const glm::vec3& position, const glm::vec3& scale)
{
	float dx = scale.x;
	float dy = scale.y;
	float dz = scale.z;
	std::vector<glm::vec3> vertices = {
		{-0.5f * dx, -0.5f * dy, -0.5f * dz}, { 0.5f * dx, -0.5f * dy, -0.5f * dz}, { 0.5f * dx,  0.5f * dy, -0.5f * dz},
		{ 0.5f * dx,  0.5f * dy, -0.5f * dz}, {-0.5f * dx,  0.5f * dy, -0.5f * dz}, {-0.5f * dx, -0.5f * dy, -0.5f * dz},

		{-0.5f * dx, -0.5f * dy,  0.5f * dz}, { 0.5f * dx, -0.5f * dy,  0.5f * dz}, { 0.5f * dx,  0.5f * dy,  0.5f * dz},
		{ 0.5f * dx,  0.5f * dy,  0.5f * dz}, {-0.5f * dx,  0.5f * dy,  0.5f * dz}, {-0.5f * dx, -0.5f * dy,  0.5f * dz},

		{-0.5f * dx,  0.5f * dy,  0.5f * dz}, {-0.5f * dx,  0.5f * dy, -0.5f * dz}, {-0.5f * dx, -0.5f * dy, -0.5f * dz},
		{-0.5f * dx, -0.5f * dy, -0.5f * dz}, {-0.5f * dx, -0.5f * dy,  0.5f * dz}, {-0.5f * dx,  0.5f * dy,  0.5f * dz},

		{ 0.5f * dx,  0.5f * dy,  0.5f * dz}, { 0.5f * dx,  0.5f * dy, -0.5f * dz}, { 0.5f * dx, -0.5f * dy, -0.5f * dz},
		{ 0.5f * dx, -0.5f * dy, -0.5f * dz}, { 0.5f * dx, -0.5f * dy,  0.5f * dz}, { 0.5f * dx,  0.5f * dy,  0.5f * dz},

		{-0.5f * dx, -0.5f * dy, -0.5f * dz}, { 0.5f * dx, -0.5f * dy, -0.5f * dz}, { 0.5f * dx, -0.5f * dy,  0.5f * dz},
		{ 0.5f * dx, -0.5f * dy,  0.5f * dz}, {-0.5f * dx, -0.5f * dy,  0.5f * dz}, {-0.5f * dx, -0.5f * dy, -0.5f * dz},

		{-0.5f * dx,  0.5f * dy, -0.5f * dz}, { 0.5f * dx,  0.5f * dy, -0.5f * dz}, { 0.5f * dx,  0.5f * dy,  0.5f * dz},
		{ 0.5f * dx,  0.5f * dy,  0.5f * dz}, {-0.5f * dx,  0.5f * dy,  0.5f * dz}, {-0.5f * dx,  0.5f * dy, -0.5f * dz}
	};
	for (glm::vec3& v : vertices)
	{
		v = v + position;
	}
	return vertices;
}
