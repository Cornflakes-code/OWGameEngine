#include "BoundingFrustum.h"
#include "BoundingBox.h"

bool Plane::isOnOrForwardPlane(const AABB& box) const
{
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	const glm::vec3& extent = box.extent();
	float r = extent.x * std::abs(normal().x) + extent.y * std::abs(normal().y) + extent.z * std::abs(normal().z);
	return -r <= getSignedDistanceToPlane(box.center());
}

bool BoundingFrustum::intersects(const AABB& box) const
{
	return topFace.isOnOrForwardPlane(box) && bottomFace.isOnOrForwardPlane(box) &&
		rightFace.isOnOrForwardPlane(box) && leftFace.isOnOrForwardPlane(box) &&
		farFace.isOnOrForwardPlane(box) && nearFace.isOnOrForwardPlane(box);
}

/*
constexpr float _max = std::numeric_limits<float>::max();

glm::vec4 Plane::ClosestPointOnPlane(const glm::vec4& p)
{
	// Not a bad site for beginner vectors
	// https://gdbooks.gitbooks.io/3dcollisions/content/Chapter1/closest_point_on_plane.html
	//// This works assuming plane.Normal is normalized, which it should be
	//float distance = DOT(plane.Normal, point) - plane.Distance;
	//// If the plane normal wasn't normalized, we'd need this:
	//// distance = distance / DOT(plane.Normal, plane.Normal);
	return glm::vec4(0.0);
}
*/
