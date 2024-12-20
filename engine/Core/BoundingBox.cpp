#include "BoundingBox.h"

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

// And thanks to https://gist.github.com/podgorskiy/308f0438a77ddee78cc12148ee654e95
// for some good ideas.

// And the following is also good
// https://github.com/iauns/cpm-glm-aabb/blob/master/glm-aabb/AABB.hpp

AABB::AABB(const std::vector<glm::vec3>& v)
{
	// Note: No depth value: default to 0;
	mMinPoint = glm::vec3(_max);
	mMaxPoint = glm::vec3(-_max);

	for (const glm::vec3& point : v)
	{
		mMinPoint = glm::min(point, mMinPoint);
		mMaxPoint = glm::max(point, mMaxPoint);
	}
}

AABB::AABB(const std::vector<glm::vec4>& v)
	:AABB(convertToV3(v))
{
}

AABB::AABB(const std::vector<AABB>& v)
{
	mMinPoint = glm::vec3(_max);
	mMaxPoint = glm::vec3(-_max);
	for (const AABB& ab : v)
	{
		mMinPoint = glm::min(ab.minPoint(), mMinPoint);
		mMaxPoint = glm::max(ab.maxPoint(), mMaxPoint);
	}
}

bool AABB::intersects(const OWBounding* other) const
{
	return false;
}

bool AABB::intersects(const AABB& other) const
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

Compass::Direction AABB::intersectionDirection(const AABB& other) const
{
	if (!intersects(other))
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

std::vector<glm::vec3> convertToV3(const std::vector<glm::vec4>& v4)
{
	std::vector<glm::vec3> v3;
	for (glm::vec4 v : v4)
	{
		v3.push_back({v.x, v.y, v.z});
	}
	return v3;
}

std::vector<std::vector<glm::vec3>> AABB::surfaces() const
{
	std::vector<std::vector<glm::vec3>> retval;
	std::vector<glm::vec3> surface;
	// front
	surface.push_back(glm::vec3({ mMinPoint.x, mMinPoint.y, mMinPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMinPoint.y, mMinPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMaxPoint.y, mMinPoint.z }));
	surface.push_back(glm::vec3({ mMinPoint.x, mMaxPoint.y, mMinPoint.z }));
	retval.push_back(surface);
	surface.clear();
	// back
	surface.push_back(glm::vec3({ mMinPoint.x, mMinPoint.y, mMaxPoint.z }));
	surface.push_back(glm::vec3({ mMinPoint.x, mMaxPoint.y, mMaxPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMaxPoint.y, mMaxPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMinPoint.y, mMaxPoint.z }));
	retval.push_back(surface);
	surface.clear();
	// top
	surface.push_back(glm::vec3({ mMaxPoint.x, mMaxPoint.y, mMaxPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMaxPoint.y, mMinPoint.z }));
	surface.push_back(glm::vec3({ mMinPoint.x, mMaxPoint.y, mMinPoint.z }));
	surface.push_back(glm::vec3({ mMinPoint.x, mMaxPoint.y, mMaxPoint.z }));
	retval.push_back(surface);
	surface.clear();
	// bottom
	surface.push_back(glm::vec3({ mMinPoint.x, mMinPoint.y, mMinPoint.z }));
	surface.push_back(glm::vec3({ mMinPoint.x, mMinPoint.y, mMaxPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMinPoint.y, mMaxPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMinPoint.y, mMinPoint.z }));
	retval.push_back(surface);
	surface.clear();
	// left
	surface.push_back(glm::vec3({ mMinPoint.x, mMinPoint.y, mMinPoint.z }));
	surface.push_back(glm::vec3({ mMinPoint.x, mMaxPoint.y, mMinPoint.z }));
	surface.push_back(glm::vec3({ mMinPoint.x, mMaxPoint.y, mMaxPoint.z }));
	surface.push_back(glm::vec3({ mMinPoint.x, mMinPoint.y, mMaxPoint.z }));
	retval.push_back(surface);
	surface.clear();
	// right
	surface.push_back(glm::vec3({ mMaxPoint.x, mMaxPoint.y, mMaxPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMinPoint.y, mMaxPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMinPoint.y, mMinPoint.z }));
	surface.push_back(glm::vec3({ mMaxPoint.x, mMaxPoint.y, mMinPoint.z }));
	retval.push_back(surface);
	return retval;
}
