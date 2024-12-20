#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"
#include "OWBounding.h"

class OWENGINE_API BoundingSphere : public OWBounding
{
	glm::vec3 mPosition = glm::vec3(0);
	float mRadius = 0.0f;
public:
	BoundingSphere() {}
	BoundingSphere(const glm::vec3& _position, float _radius)
		:mPosition(_position), mRadius(_radius) {}
	void radius(float _radius) { mRadius = _radius; }
	void center(const glm::vec3& _value) { mPosition = _value; }
	void move(const glm::vec3& delta) { mPosition += delta; }
	bool intersects(const OWBounding* other) const override;
	bool intersects(const AABB& box) const;
	bool contains(const glm::vec3& pt) const
	{
		glm::vec3 diff = mPosition - pt;
		return mRadius > glm::abs(glm::length(diff));
	}
};
