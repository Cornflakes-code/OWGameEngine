#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"
#include "OWShape.h"

class OWENGINE_API OWSphere: public OWShape
{
	glm::vec3 mOrigin;
	float mRadius;
	void drawSphere(int lats, int longs);
public:
	OWSphere(const glm::vec3& _origin, float _radius)
		: mOrigin(_origin), mRadius(_radius)
	{ }
	bool intersects(const AABB& box, glm::vec3& normal, float& distance) const;
	std::vector<glm::vec3> vertices() override;
};
