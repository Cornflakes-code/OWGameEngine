#include "BoundingBox.h"

glm::vec4 convert(const glm::vec3& v3)
{
	glm::vec4 retval = glm::vec4(v3, 1);
	return retval;
}

glm::vec3 convert(const glm::vec4& v4)
{
	glm::vec3 retval = glm::vec3(v4.x, v4.y, v4.z);
	return retval;
}

AABBV4 convertToV4(const AABBV3& v3)
{
	glm::vec4 _min = convert(v3.minPoint());
	glm::vec4 _max = convert(v3.maxPoint());
	return AABBV4(_min, _max);
}

AABBV3 convertToV3(const AABBV4& v4)
{
	glm::vec3 _min = convert(v4.minPoint());
	glm::vec3 _max = convert(v4.maxPoint());
	return AABBV3(_min, _max);
}


namespace Compass
{
	glm::vec4 Rose[NumDirections] = {
			glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),	// North
			glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),	// South
			glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),	// East
			glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f),	// West
			glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),	// in (maybe wrong)
			glm::vec4(1.0f, 0.0f, -1.0f, 0.0f),	// out (maybe wrong)
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)	// No Direction
	};

	std::string asString(Direction dir)
	{
		switch (dir)
		{
		case North: return "North";
		case South: return "South";
		case East: return "East";
		case West: return "West";
		case In: return "In";
		case Out: return "Out";
		case NoDirection: return "NoDirection";
		case NumDirections: return "NumDirections";
		default: throw NMSLogicException(std::stringstream() << "Unknown Direction [" << dir << "]\n");
		}
	}
}

/*
#include <glm/gtx/intersect.hpp>

#include "ErrorHandling.h"

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

template <int Dim, typename Type>
AABB<Dim, Type>::AABB<Dim, Type>(const std::vector<vec_type>& v)
{
	// Note: No depth value: default to 0;
	mMinPoint = vec_type(_max);
	mMaxPoint = vec_type(-_max, -_max, -_max, 1);

	for (const vec_type& point : v)
	{
		if (point.x < mMinPoint.x)
			mMinPoint.x = point.x;
		if (point.x > mMaxPoint.x)
			mMaxPoint.x = point.x;

		if (point.y < mMinPoint.y)
			mMinPoint.y = point.y;
		if (point.y > mMaxPoint.y)
			mMaxPoint.y = point.y;

		if (point.z < mMinPoint.z)
			mMinPoint.z = point.z;
		if (point.z > mMaxPoint.z)
			mMaxPoint.z = point.z;

		if (point.w < mMinPoint.w)
			mMinPoint.w = point.w;
		if (point.w > mMaxPoint.w)
			mMaxPoint.w = point.w;
	}
}

template <int Dim, typename Type>
AABB<Dim, Type>::AABB<Dim, Type>(const std::vector<AABB<Dim, Type>>& v)
{
}

template <int Dim, typename Type>
void AABB<Dim, Type>::isValid() const
{
	if (mMinPoint.x > mMaxPoint.x || mMinPoint.y > mMaxPoint.y || mMinPoint.z > mMaxPoint.z)
		throw NMSLogicException("Error: Invalid AABB size.");
}

template <int Dim, typename Type>
bool AABB<Dim, Type>::overlap(const AABB<Dim, Type>& other) const
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

// returns the AABB that encompasses both params
template <int Dim, typename Type>
AABB<Dim, Type> operator+(const AABB<Dim, Type>& left, const AABB<Dim, Type>& right)
{
	vec_type minPoint = vec_type(std::min(left.minPoint().x, right.minPoint().x),
		std::min(left.minPoint().y, right.minPoint().y),
		std::min(left.minPoint().z, right.minPoint().z),
		1.0);
	vec_type maxPoint = glm::vec4(std::max(left.maxPoint().x, right.maxPoint().x),
		std::max(left.maxPoint().y, right.maxPoint().y),
		std::max(left.maxPoint().z, right.maxPoint().z),
		1.0);
	return AABB<Dim, Type>(minPoint, maxPoint);
}

bool Ball::contains(const AABB<Dim, Type>& other) const
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
*/