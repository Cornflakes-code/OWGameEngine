#include "BoundingPlane.h"

bool BoundingPlane::intersects(const OWBounding* other) const
{
	return false;
}

void BoundingPlane::move(const glm::vec3& pt)
{

}

bool BoundingPlane::isOnOrForwardPlane(const AABB& box) const
{
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	const glm::vec3& extent = box.extent();
	float r = extent.x * std::abs(mNormal.x) + extent.y * std::abs(mNormal.y) + extent.z * std::abs(mNormal.z);
	return -r <= getSignedDistanceToPlane(box.center());
}
