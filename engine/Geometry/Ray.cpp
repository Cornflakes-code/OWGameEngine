#include "Ray.h"

Ray::Ray(OWActor* _owner, const glm::vec3& origin, const glm::vec3& dir)
	: OWSceneComponent(_owner, origin), mDirection(dir)
{
	mInvDir = 1.0f / glm::normalize(mDirection);
	mSign[0] = (mInvDir.x < 0) ? 1 : 0;
	mSign[1] = (mInvDir.y < 0) ? 1 : 0;
	mSign[2] = (mInvDir.z < 0) ? 1 : 0;
}

static glm::vec3 findNormal(float distance, float t1, float t2, float t3, float t4, float t5, float t6)
{
	if (glm::epsilonEqual(distance, t1, OWUtils::epsilon()))
		return glm::vec3(1, 0, 0);
	else if (glm::epsilonEqual(distance, t2, OWUtils::epsilon()))
		return glm::vec3(-1, 0, 0);
	else if (glm::epsilonEqual(distance, t3, OWUtils::epsilon()))
		return glm::vec3(0, 1, 0);
	else if (glm::epsilonEqual(distance, t4, OWUtils::epsilon()))
		return glm::vec3(0, -1, 0);
	else if (glm::epsilonEqual(distance, t5, OWUtils::epsilon()))
		return glm::vec3(0, 0, -1);
	else if (glm::epsilonEqual(distance, t6, OWUtils::epsilon()))
		return glm::vec3(0, 0, 1);
	else
		return glm::vec3(0, 0, 0);
}

static bool doIntersection(const glm::vec3& invDir, const glm::vec3& origin, const AABB& box, glm::vec3& normal, float& distance)
{
	// https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	// https://gamedev.stackexchange.com/questions/47888/find-the-contact-normal-of-rectangle-collision?noredirect=1&lq=1

	float t1 = (box.minPoint().x - origin.x) * invDir.x; // left of box contacted normal = -1,0,0 dir of ray = Compass::West
	float t2 = (box.maxPoint().x - origin.x) * invDir.x; // right of box contacted normal = 1,0,0 dir of ray = Compass::East
	float t3 = (box.minPoint().y - origin.y) * invDir.y; // top of box contacted normal = 0,1,0 dir of ray = Compass::South
	float t4 = (box.maxPoint().y - origin.y) * invDir.y; // bottom of box contacted normal = 0,-1,0 dir of ray = Compass::North
	float t5 = (box.minPoint().z - origin.z) * invDir.z; // +z of box contacted  normal = 0,0,1 dir of ray = Compass::In
	float t6 = (box.maxPoint().z - origin.z) * invDir.z; // -z of box contacted  normal = 0,0,-1 dir of ray = Compass::Out

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		distance = -tmax;
		normal = findNormal(distance, t1, t2, t3, t4, t5, t6);
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	else if (tmin > tmax)
	{
		normal = glm::vec3(0, 0, 0);
		distance = 0;
		return false;
	}
	else
	{
		distance = tmin;
		normal = findNormal(distance, t1, t2, t3, t4, t5, t6);
		return true;
	}
}

bool Ray::rayIntersectsBox(const glm::vec3& origin, const glm::vec3& dir, const AABB& box, glm::vec3& normal, float& distance)
{
	glm::vec3 invDir = 1.0f / glm::normalize(dir);
	return doIntersection(invDir, origin, box, normal, distance);
}

bool Ray::intersects(const AABB& box, glm::vec3& normal, float& distance) const
{
	return doIntersection(mInvDir, position(), box, normal, distance);
}

bool Ray::intersects1(const AABB& box) const
{
	glm::vec3 bounds[2];
	bounds[0] = box.minPoint();
	bounds[1] = box.maxPoint();
	float tmin = (bounds[mSign[0]].x - position().x) * mInvDir.x;
	float tmax = (bounds[1 - mSign[0]].x - position().x) * mInvDir.x;
	float tymin = (bounds[mSign[1]].y - position().y) * mInvDir.y;
	float tymax = (bounds[1 - mSign[1]].y - position().y) * mInvDir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (bounds[mSign[2]].z - position().z) * mInvDir.z;
	float tzmax = (bounds[1 - mSign[2]].z - position().z) * mInvDir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	return true;
}
