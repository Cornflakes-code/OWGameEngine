#include "BoundingBox.h"

#include <glm/gtx/intersect.hpp>

#include "ErrorHandling.h"

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

//__cdecl AABB::AABB(struct glm::vec<4,float,0> const &,struct glm::vec<4,float,0> const &)" (? ? 0AABB@@QEAA@AEBU?$vec@$03M$0A@@glm@@0@Z) referenced in function "private: class AABB __cdecl TextBillboard::findBounds(void)const " (? findBounds@TextBillboard@@AEBA?AVAABB@@XZ)
AABB::AABB(const glm::vec4& _minPoint, const glm::vec4& _maxPoint, bool validate)
	: mMinPoint(_minPoint), mMaxPoint(_maxPoint)
{
	if (validate)
		isValid();
}

AABB::AABB()
: mMinPoint(glm::vec4(0.0, 0.0, 0.0, 1.0)), mMaxPoint(glm::vec4(0.0, 0.0, 0.0, 1.0))
{
	isValid();
}

AABB AABB::calcBounds(const std::vector<glm::vec4>& v)
{
	// Note: No depth value: default to 0;
	const float _max = std::numeric_limits<float>::max();
	glm::vec4 minPoint(_max, _max, 0, 1);
	glm::vec4 maxPoint(-_max, -_max, 0, 1);

	for (const auto& point : v)
	{
		if (point.x < minPoint.x)
			minPoint.x = point.x;
		else if (point.x > maxPoint.x)
			maxPoint.x = point.x;
		else if (point.y < minPoint.y)
			minPoint.y = point.y;
		else if (point.y > maxPoint.y)
			maxPoint.y = point.y;
	}
	return AABB(minPoint, maxPoint);
}

void AABB::isValid() const
{
	if (mMinPoint.x > mMaxPoint.x || mMinPoint.x > mMaxPoint.y || mMinPoint.z > mMaxPoint.z)
		throw NMSLogicException("Error: Invalid AABBV size.");

}

bool AABB::overlap(const AABB& other) const
{
	// If there is no overlap in any one direction 
	// then the objects cannot intersect
	if (mMinPoint.x <= other.mMaxPoint.x &&
		mMaxPoint.x >= other.mMinPoint.x)
		return true;
	if (mMinPoint.y <= other.mMaxPoint.y &&
		mMaxPoint.y >= other.mMinPoint.y)
		return true;
	if (mMinPoint.z <= other.mMaxPoint.z &&
		mMaxPoint.z >= other.mMinPoint.z)
		return true;
	return false;
}

Compass::Direction AABB::wallIntersection(const AABB& other) const
{
	if (!overlap(other))
		return Compass::Direction::NoDirection;
	if (other.maxPoint().y >= maxPoint().y)
		return Compass::Direction::North;
	else if (other.maxPoint().x >= maxPoint().x)
		return Compass::Direction::East;
	else if (other.maxPoint().z >= maxPoint().z)
		return Compass::Direction::In;
	else if (other.minPoint().y <= minPoint().y)
		return Compass::Direction::South;
	else if (other.minPoint().x <= minPoint().x)
		return Compass::Direction::West;
	else if (other.minPoint().z <= minPoint().z)
		return Compass::Direction::Out;
	return Compass::Direction::NoDirection;
}

glm::vec4 AABB::pointAndNormal(Compass::Direction dir, glm::vec4& normal)
{
	glm::vec4 pointOnPlane = center();
	if (dir == Compass::North)
	{
		normal.x = 0.0;
		normal.y = 1.0;
		normal.z = 0.0;
		pointOnPlane.y += size().y / 2;
	}
	else if (dir == Compass::South)
	{
		normal.x = 0.0;
		normal.y = -1.0;
		normal.z = 0.0;
		pointOnPlane.y -= size().y / 2;
	}
	else if (dir == Compass::East)
	{
		normal.x = 1.0;
		normal.y = 0.0;
		normal.z = 0.0;
		pointOnPlane.x += size().x / 2;
	}
	else if (dir == Compass::West)
	{
		normal.x = -1.0;
		normal.y = 0.0;
		normal.z = 0.0;
		pointOnPlane.x -= size().x / 2;
	}
	else if (dir == Compass::In)
	{
		normal.x = 0.0;
		normal.y = 0.0;
		normal.z = 1.0;
		pointOnPlane.z += size().z / 2;
	}
	else if (dir == Compass::Out)
	{
		normal.x = 0.0;
		normal.y = 0.0;
		normal.z = -1.0;
		pointOnPlane.z -= size().z / 2;
	}
	normal.w = 0.0;

	return pointOnPlane;
}

glm::vec4 AABB::calcBouncePosition(const glm::vec4& origen, const glm::vec4& dest, Compass::Direction whichWall)
{
	glm::vec4 normToEdge;
	glm::vec4 pointOnEdge = pointAndNormal(whichWall, normToEdge);

	OWUtils::Float distToPlane = 0.0;
	glm::vec4 distance = dest - origen;
	glm::vec4 direction = glm::normalize(dest - origen);
	if (glm::intersectRayPlane(origen, direction, pointOnEdge, normToEdge, distToPlane))
	{
		glm::vec4 intersectionPoint = origen + direction * distToPlane;
		glm::vec4 remainingDistAfterBounce = distance - direction * distToPlane;
		if (whichWall == Compass::Direction::North || whichWall == Compass::Direction::South)
			direction.y = -direction.y;
		else if (whichWall == Compass::Direction::East || whichWall == Compass::Direction::West)
			direction.x = -direction.x;
		else if (whichWall == Compass::Direction::In || whichWall == Compass::Direction::Out)
			direction.z = -direction.z;
		glm::vec4 positionAfterBounce = intersectionPoint + direction * remainingDistAfterBounce;
		return glm::vec4(origen - positionAfterBounce);
	}
	else
		throw NMSLogicException("Object should have intersected Boundary");
}

// returns the AABB that encompasses both params
AABB operator+(const AABB& left, const AABB& right)
{
	glm::vec4 minPoint = glm::vec4(std::min(left.minPoint().x, right.minPoint().x),
		std::min(left.minPoint().y, right.minPoint().y),
		std::min(left.minPoint().z, right.minPoint().z),
		1.0);
	glm::vec4 maxPoint = glm::vec4(std::max(left.maxPoint().x, right.maxPoint().x),
		std::max(left.maxPoint().y, right.maxPoint().y),
		std::max(left.maxPoint().z, right.maxPoint().z),
		1.0);
	return AABB(minPoint, maxPoint);
}
//AABB operator-(const AABB& left, const AABB& right)
//{
//	AABB retval = left;
//	retval.mMinPoint -= right.mMinPoint;
//	retval.mMaxPoint -= right.mMaxPoint;
//	return retval;
//}
//
//AABB operator+(const AABB& left, const AABB& right)
//{
//	AABB retval = left;
//	retval.mMinPoint += right.mMinPoint;
//	retval.mMaxPoint += right.mMaxPoint;
//	return retval;
//}

bool Ball::contains(const AABB& other) const
{
	// https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
	glm::vec4 ballCenter = center();

	// calculate AABB info (center, half-extents)
	glm::vec4 aabb_half_extents = other.size() / 2.0f;
	glm::vec4 aabb_center = other.center();

	// get difference vector between both centers
	glm::vec4 difference = ballCenter - aabb_center;
	glm::vec4 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	// add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec4 closest = aabb_center + clamped;

	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - ballCenter;

	return glm::length(difference) < mRadius;
}
