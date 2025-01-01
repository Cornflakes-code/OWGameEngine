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
	if (!glm::all(glm::epsilonEqual(delta, glm::vec3(0), OWUtils::epsilon())))
		throw NMSNotYetImplementedException("Object [Bounding Plane::move] not implemented");
	//mDistance += delta;
}

void BoundingPlane::moveTo(const glm::vec3& pt)
{
	if (!glm::all(glm::epsilonEqual(pt, glm::vec3(0), OWUtils::epsilon())))
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
