#pragma once

#include <glm/glm.hpp>

#include "BoundingBox.h"
#include "OWBounding.h"

#include "../OWEngine/OWEngine.h"

class OWENGINE_API BoundingPlane: public OWBounding
{
	glm::vec3 mNormal = { 0.f, 1.f, 0.f }; // unit vector
	float mDistance = 0.f; // distance from origin
public:
	BoundingPlane()
		: BoundingPlane(glm::vec3(0), 0.0f) {}

	BoundingPlane(const glm::vec3& _normal, float _distance)
		: mNormal(_normal), mDistance(_distance)
	{}

	BoundingPlane(const glm::vec3& norm, const glm::vec3& p1)
	: mNormal(glm::normalize(norm))
	, mDistance(glm::dot(mNormal, p1)) {}

	bool intersects(const OWBounding* other) const override;
	void move(const glm::vec3& delta) override;
	void moveTo(const glm::vec3& pt) override;
	const glm::vec3& normal() const { return mNormal; }
	float getSignedDistanceToPlane(const glm::vec3& point) const
	{
		return glm::dot(mNormal, point) - mDistance;
	}

	bool isOnOrForwardPlane(const AABB& box) const;
};
