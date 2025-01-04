#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"
#include "OWShape.h"

class OWENGINE_API OWRay: public OWShape
{
	glm::vec3 mOrigin;
	glm::vec3 mDirection;
	glm::vec3 mInvDir;
	bool internalIntersects(const AABB& box, glm::vec3& normal, float& distance);
	bool externalIntersects(const AABB& box, glm::vec3& normal, float& distance);
	bool fasterExternalIntersects(const AABB& box, glm::vec3& normal, float& distance);
public:
	OWRay(const glm::vec3& _origin, const glm::vec3& _direction);
	bool intersects(const AABB& box, glm::vec3& normal, float& distance);
	std::vector<glm::vec3> vertices() const override;
};
