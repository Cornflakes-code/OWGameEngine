#include "Box.h"
#include "GeometricShapes.h"

Box::Box(OWActor* _owner, const glm::vec3& _position, const glm::quat& q)
	: OWSceneComponent(_owner, _position), mQuat(q)
{}

bool Box::intersects(const AABB& box) const
{
	return false;
}

void Box::prepare()
{
	std::vector<glm::vec3> vertices = GeometricShapes::cube();
}
