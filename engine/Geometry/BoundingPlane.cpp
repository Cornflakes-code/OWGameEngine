#include "BoundingPlane.h"
#include <glm/gtc/epsilon.hpp>

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

bool BoundingPlane::intersects(const OWBounding* other) const
{
	return false;
}

void BoundingPlane::move(const glm::vec3& delta)
{
	if (!OWUtils::isZero(delta))
		throw NMSNotYetImplementedException("Object [Bounding Plane::move] not implemented");
	//mDistance += delta;
}

void BoundingPlane::move(const glm::vec4& delta)
{
	if (!OWUtils::isZero(delta))
		throw NMSNotYetImplementedException("Object [Bounding Plane::move] not implemented");
	//mDistance += delta;
}


void BoundingPlane::moveTo(const glm::vec3& pt)
{
	if (!OWUtils::isZero(pt))
		throw NMSNotYetImplementedException("Object [Bounding Plane::moveTo] not implemented");
	//mDistance = pt;
}


bool BoundingPlane::isOnOrForwardPlane(const AABB& box) const
{
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	const glm::vec3& extent = box.extent();
	float r = extent.x * std::abs(mNormal.x) + extent.y * std::abs(mNormal.y) + extent.z * std::abs(mNormal.z);
	return -r <= getSignedDistanceToPlane(box.center());
}
