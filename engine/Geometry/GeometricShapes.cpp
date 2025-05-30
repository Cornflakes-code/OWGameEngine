#include "GeometricShapes.h"
#include <algorithm>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Core/CommonUtils.h>

std::pair<glm::vec3, glm::vec3> OWGeometricShapes::minMaxBox =
{
	glm::vec3(std::numeric_limits<float>::min(),
			std::numeric_limits<float>::min(),
			std::numeric_limits<float>::min()),
	glm::vec3(std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max())
};

std::vector<glm::vec3> OWGeometricShapes::circle(float arcRadians)
{
	float radius = 1.0f;
	std::vector<glm::vec3> retval;
	for (float i = 0.0f; i < glm::two_pi<float>(); i += arcRadians)
	{
		retval.push_back({ radius * glm::cos(i), radius * glm::sin(i), 0 });
	}
	return retval;
}

std::vector<glm::vec3> OWGeometricShapes::torus(float innerRadius,
										float outerRadius,
										float arcRadians)
{
	std::vector<glm::vec3> retval;
	for (float i = 0.0f; i < glm::two_pi<float>(); i += arcRadians)
	{
		float arc = i + arcRadians;

		retval.push_back({ innerRadius * glm::cos(i), innerRadius * glm::sin(i), 0 });
		retval.push_back({ outerRadius * glm::cos(i), outerRadius * glm::sin(i), 0 });
		retval.push_back({ outerRadius * glm::cos(arc), outerRadius * glm::sin(arc), 0 });

		retval.push_back({ innerRadius * glm::cos(i), innerRadius * glm::sin(i), 0 });
		retval.push_back({ outerRadius * glm::cos(arc), outerRadius * glm::sin(arc), 0 });
		retval.push_back({ innerRadius * glm::cos(arc), innerRadius * glm::sin(arc), 0 });
	}
	return retval;
}

std::vector<glm::vec3> OWGeometricShapes::rectangle(const glm::vec2& dims)
{
	const float x = dims.x * 0.5f;
	const float y = dims.y * 0.5f;
	float z = 0.0f;
	std::vector<glm::vec3> retval =
	{
		{ x, y, z },
		{ x, -y, z },
		{ -x, -y, z },
		{ -x, -y, z },
		{ -x, y, z },
		{ x, y, z }
	};

	return retval;
}

std::vector<glm::vec3> OWGeometricShapes::goldenRectangle()
{
	// Create at {0,0}, scale then move.
	float magicGoldenNumber = (89.0f / 55.0f) / 2.0f;
	float ydim = 0.5f;
	std::vector<glm::vec3> retval;
	retval.push_back(glm::vec3(-magicGoldenNumber, -ydim, 0));
	retval.push_back(glm::vec3(-magicGoldenNumber, ydim, 0));
	retval.push_back(glm::vec3(magicGoldenNumber, ydim, 0));
	retval.push_back(glm::vec3(magicGoldenNumber, -ydim, 0));
	return retval;
}

// But the points are bunched near the poles.
std::vector<glm::vec3> OWGeometricShapes::pointsOnSphere(
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
std::vector<glm::vec3> OWGeometricShapes::fibonacciSphere(int numPoints, float TAU)
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

std::vector<glm::vec3> OWGeometricShapes::star(float innerRadius, float outerRadius,
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


std::vector<glm::vec3> OWGeometricShapes::tetrahedron()
{
	const float p = 1.0f;
	const std::vector<glm::vec3> tetStart =
	{
		{ p, p, p }, { p, -p, -p }, { -p, p, -p }, { -p, -p, p }
	};

	std::vector<glm::vec3> tetTriangles =
	{
		tetStart[0], tetStart[1], tetStart[2],
		tetStart[1], tetStart[2], tetStart[3],
		tetStart[2], tetStart[3], tetStart[0],
		tetStart[3], tetStart[0], tetStart[1]
	};
	return tetTriangles;
}

std::vector<glm::vec3> OWGeometricShapes::cube()
{
	const glm::vec3& scale = { 1.0f, 1.0f, 1.0f };
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
	return vertices;
}

std::vector<glm::vec3> OWGeometricShapes::line(float thickness)
{
	return { { -0.5f, 0, 0 }, { 0.5f, 0, 0 } };
}

std::vector<glm::vec3> OWGeometricShapes::beam(const glm::vec3& direction, float length)
{
	glm::vec3 beamEnd = glm::normalize(direction) * length;
	return OWGeometricShapes::beam(beamEnd);
}

std::vector<glm::vec3> OWGeometricShapes::beam(const glm::vec3& beamEnd_)
{
	// https://github.com/atomic/OpenGL/blob/master/openGLexamples/tetrahedron.cpp

	const glm::vec3 beamStart = { 0.0f, 0.0f, 0.0f };
	glm::vec3 beamEnd = beamEnd_;
	//glm::vec3 beamEnd = beamEnd_ / 2.0f;
	//const glm::vec3 beamStart = -beamEnd;
	glm::vec3 lineDir = glm::normalize(beamStart - beamEnd);
	glm::vec3 anotherDir = glm::vec3(1.0f, 0.0f, 0.0f);
	if (OWUtils::isZero(lineDir - anotherDir))
		anotherDir = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 orthogonalVector = glm::normalize(glm::cross(lineDir, anotherDir));
	float mult = 1.1f;
	glm::vec3 p1 = orthogonalVector * mult;
	
	glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(120.0f), lineDir);
	glm::vec3 p2 = glm::vec3(glm::vec4(p1, 1.0f) * rot_mat);

	rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(240.0f), lineDir);
	glm::vec3 p3 = glm::vec3(glm::vec4(p1, 1.0f) * rot_mat);

	std::vector<glm::vec3> beam;

	// Cover the end
	beam.push_back(p1 + beamStart);
	beam.push_back(p2 + beamStart);
	beam.push_back(p3 + beamStart);

// Two triangles for the first side
	beam.push_back(p1 + beamStart);
	beam.push_back(p2 + beamEnd);
	beam.push_back(p3 + beamEnd);

	beam.push_back(p2 + beamStart);
	beam.push_back(p2 + beamEnd);
	beam.push_back(p3 + beamEnd);

	// Two triangles for the second side
	beam.push_back(p2 + beamStart);
	beam.push_back(p3 + beamEnd);
	beam.push_back(p1 + beamEnd);

	beam.push_back(p3 + beamStart);
	beam.push_back(p3 + beamEnd);
	beam.push_back(p1 + beamEnd);

	// Two triangles for the third side
	beam.push_back(p3 + beamStart);
	beam.push_back(p1 + beamEnd);
	beam.push_back(p2 + beamEnd);

	beam.push_back(p1 + beamStart);
	beam.push_back(p1 + beamEnd);
	beam.push_back(p2 + beamEnd);

	// Cover the end
	//beam.push_back(p1 + beamEnd);
	//beam.push_back(p2 + beamEnd);
	//beam.push_back(p3 + beamEnd);
	return beam;
}
