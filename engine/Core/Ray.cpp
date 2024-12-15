#include "Ray.h"

Ray::Ray(const glm::vec3& orig, const glm::vec3& dir)
	: mOrigen(orig), mDirection(dir)
{
	mInvDir = 1.0f / mDirection;
	mSign[0] = (mInvDir.x < 0) ? 1 : 0;
	mSign[1] = (mInvDir.y < 0) ? 1 : 0;
	mSign[2] = (mInvDir.z < 0) ? 1 : 0;
}

bool Ray::intersects(const AABB& box) const
{
	glm::vec3 bounds[2];
	bounds[0] = box.minPoint();
	bounds[1] = box.maxPoint();
	float tmin = (bounds[mSign[0]].x - mOrigen.x) * mInvDir.x;
	float tmax = (bounds[1 - mSign[0]].x - mOrigen.x) * mInvDir.x;
	float tymin = (bounds[mSign[1]].y - mOrigen.y) * mInvDir.y;
	float tymax = (bounds[1 - mSign[1]].y - mOrigen.y) * mInvDir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (bounds[mSign[2]].z - mOrigen.z) * mInvDir.z;
	float tzmax = (bounds[1 - mSign[2]].z - mOrigen.z) * mInvDir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	return true;
}

void Ray::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const glm::vec3& cameraPos)
{}
