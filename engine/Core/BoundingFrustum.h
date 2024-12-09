#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
/*
template <int Dim, typename Type>
class OWENGINE_API BoundingFrustum
{
	typedef glm::vec glm::vec3;
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec3 mOrigen;
	glm::vec3 mDirection;
	glm::vec3 mInvDir;
	int mSign[3];
#pragma warning( pop )
public:
	BoundingFrustum(const glm::vec3& orig, const glm::vec3& dir)
		: mOrigen(orig), mDirection(dir)
	{
		mInvDir = 1.0f / mDirection;
		mSign[0] = (mInvDir.x < 0) ? 1 : 0;
		mSign[1] = (mInvDir.y < 0) ? 1 : 0;
		mSign[2] = (mInvDir.z < 0) ? 1 : 0;
	}
};
*/

class AABB;

class OWENGINE_API Plane
{
	glm::vec3 mNormal = { 0.f, 1.f, 0.f }; // unit vector
	float mDistance = 0.f; // distance from origin
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	Plane() = default;
	Plane(const glm::vec3& p1, const glm::vec3& norm)
		: mNormal(glm::normalize(norm)),
		mDistance(glm::dot(mNormal, p1))
	{}
	const glm::vec3& normal() const { return mNormal; }
	float getSignedDistanceToPlane(const glm::vec3& point) const
	{
		return glm::dot(mNormal, point) - mDistance;
	}

	bool isOnOrForwardPlane(const AABB& box) const;

#pragma warning( pop )
};

class OWENGINE_API BoundingFrustum
{
public:
	Plane topFace;
	Plane bottomFace;

	Plane rightFace;
	Plane leftFace;

	Plane farFace;
	Plane nearFace;
	bool intersects(const AABB& box) const;
};
