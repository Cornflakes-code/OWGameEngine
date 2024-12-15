#pragma once

#include <glm/glm.hpp>

#include "Plane.h"
#include "BoundingBox.h"

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