#include "OWRay.h"

OWRay::OWRay(const glm::vec3& _origin, const glm::vec3& _direction)
	: mOrigin(_origin), mDirection(glm::normalize(_direction))
{
	mInvDir = 1.0f / mDirection;
	mVertices.push_back(_origin);
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

/*
* https://stackoverflow.com/questions/1585525/how-to-find-the-intersection-point-between-a-line-and-a-rectangle
	*Finds the intersection point between
	** a rectangle centered in point B
	* with sides parallel to the x and y axes
	** a line passing through points A and B(the center of the rectangle)
	*
	* @param width : rectangle width
	* @param height : rectangle height
	* @param xB; rectangle center x coordinate
	* @param yB; rectangle center y coordinate
	* @param xA; point A x coordinate
	* @param yA; point A y coordinate
	* @author Federico Destefanis
	* @see <a href = "https://stackoverflow.com/a/31254199/2668213">based on< / a>
*/

bool OWRay::internalIntersects(const AABB& box, glm::vec3& normal, float& distance)
{
	// https://stackoverflow.com/questions/51252535/exit-point-of-an-arbitrary-ray-within-a-cube
	glm::vec3 dim = box.size() / 2.0f;
	glm::vec3 point = dim * mInvDir;
	if (point.x > 0 && point.y > 0)
		normal = { 1,0,0 };
//	float width = box.maxPoint().x - box.minPoint().x;
//	float height = box.maxPoint().y - box.minPoint().y;
	glm::vec3 center = box.center();
	return false;
}

bool OWRay::fasterExternalIntersects(const AABB& box, glm::vec3& normal, float& distance)
{
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html

	/*
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	int sign[3];
	sign[0] = (mInvDir.x < 0);
	sign[1] = (mInvDir.y < 0);
	sign[2] = (mInvDir.z < 0);
	tmin = (bounds[r.sign[0]].x - r.orig.x) * r.invdir.x;
	tmax = (bounds[1 - r.sign[0]].x - r.orig.x) * r.invdir.x;
	tymin = (bounds[r.sign[1]].y - r.orig.y) * r.invdir.y;
	tymax = (bounds[1 - r.sign[1]].y - r.orig.y) * r.invdir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[r.sign[2]].z - r.orig.z) * r.invdir.z;
	tzmax = (bounds[1 - r.sign[2]].z - r.orig.z) * r.invdir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return true;
	*/
	return false;
}

bool OWRay::externalIntersects(const AABB& box, glm::vec3& normal, float& distance)
{
	// https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	// https://gamedev.stackexchange.com/questions/47888/find-the-contact-normal-of-rectangle-collision?noredirect=1&lq=1

	float t1 = (box.minPoint().x - mOrigin.x) * mInvDir.x; // left of box contacted normal = -1,0,0 dir of ray = Compass::West
	float t2 = (box.maxPoint().x - mOrigin.x) * mInvDir.x; // right of box contacted normal = 1,0,0 dir of ray = Compass::East
	float t3 = (box.minPoint().y - mOrigin.y) * mInvDir.y; // top of box contacted normal = 0,1,0 dir of ray = Compass::South
	float t4 = (box.maxPoint().y - mOrigin.y) * mInvDir.y; // bottom of box contacted normal = 0,-1,0 dir of ray = Compass::North
	float t5 = (box.minPoint().z - mOrigin.z) * mInvDir.z; // +z of box contacted  normal = 0,0,1 dir of ray = Compass::In
	float t6 = (box.maxPoint().z - mOrigin.z) * mInvDir.z; // -z of box contacted  normal = 0,0,-1 dir of ray = Compass::Out

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


bool OWRay::intersects(const AABB& box, glm::vec3& normal, float& distance)
{
	if (false)//box.contains(mOrigin))
	{
		return internalIntersects(box, normal, distance);
	}
	else
	{
		return externalIntersects(box, normal, distance);
	}
}

std::vector<glm::vec3> OWRay::vertices() const
{
	return mVertices;
}
