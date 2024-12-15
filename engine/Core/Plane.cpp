#include "Plane.h"
#include "BoundingBox.h"
#include "ErrorHandling.h"

Plane::Plane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4)
{
//	throw NMSException(std::string("Warning. The rest of this class defines infinite planes") +
//				"The intention of this ctor is to define a finite plane.");
	mNormal = glm::cross(glm::vec3(p4 - p3), glm::vec3(p2 - p3));
	mDistance = glm::length(glm::vec3(p1 + p2 + p3 + p4) / 4.0f);
}

Plane::Plane(const std::vector<glm::vec3>& corners)
	: Plane(corners[0], corners[1], corners[2], corners[3] )
{
}

glm::vec3 mNormal = { 0.f, 1.f, 0.f }; // unit vector
float mDistance = 0.f; // distance from origin

void Plane::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const glm::vec3& cameraPos)
{
}
