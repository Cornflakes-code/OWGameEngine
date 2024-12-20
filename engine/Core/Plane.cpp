#include "Plane.h"
#include "BoundingBox.h"
#include "ErrorHandling.h"

Plane::Plane(OWActor* _owner, const glm::vec3& _position, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4)
: OWSceneComponent(_owner, _position), mPlane(glm::cross(glm::vec3(p4 - p3), glm::vec3(p2 - p3)), glm::length(glm::vec3(p1 + p2 + p3 + p4) / 4.0f))
{
//	throw NMSException(std::string("Warning. The rest of this class defines infinite planes") +
//				"The intention of this ctor is to define a finite plane.");
}

Plane::Plane(OWActor* _owner, const glm::vec3& _position, const std::vector<glm::vec3>& corners)
	: Plane(_owner, _position, corners[0], corners[1], corners[2], corners[3] )
{
}
