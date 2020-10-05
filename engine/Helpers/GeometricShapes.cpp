#include "GeometricShapes.h"
#include <glm/gtc/constants.hpp>

GeometricShapes::GeometricShapes()
{
}

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
