#include "BoundingBox.h"
#include <cmath>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Compass
{
	glm::vec4 Rose[NumDirections] = {
			glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),	// North
			glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),	// South
			glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),	// East
			glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f),	// West
			glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),	// In
			glm::vec4(0.0f, 0.0f, -1.0f, 0.0f),	// Out
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
// https://gist.github.com/yomotsu/d845f21e2e1eb49f647f

AABB::AABB(const glm::vec3& size)
{
	mMinPoint = size / -2.0f;
	mMaxPoint = size / 2.0f;
}

AABB::AABB(std::vector<glm::vec3>::const_iterator begin, std::vector<glm::vec3>::const_iterator end)
{
	mMinPoint = glm::vec3(_max);
	mMaxPoint = glm::vec3(-_max);
	for (std::vector<glm::vec3>::const_iterator i = begin; i != end; ++i)
	{
		mMinPoint = glm::min(*i, mMinPoint);
		mMaxPoint = glm::max(*i, mMaxPoint);
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

void AABB::scale(const glm::vec3& factor)
{
	const glm::vec3 c = center();
	mMinPoint = (mMinPoint - c) * factor;
	mMaxPoint = (mMaxPoint - c) * factor;
}

float AABB::diagonal() const
{
	glm::vec3 span = (maxPoint() - minPoint()) / 2.0f;
	return std::sqrtf(span.x * span.x + span.y * span.y + span.z * span.z);
}

bool AABB::intersects(const OWBounding* other) const
{
	return false;
}

bool AABB::intersects(const AABB& other) const
{
	// If there is no overlap in any one direction 
	// then the objects cannot intersect
	if (mMaxPoint.x > other.mMinPoint.x &&
		other.mMaxPoint.x > mMinPoint.x &&

		mMaxPoint.y > other.mMinPoint.y &&
		other.mMaxPoint.y > mMinPoint.y &&

		mMaxPoint.z > other.mMinPoint.z &&
		other.mMaxPoint.z > mMinPoint.z)
		return true;
	return false;
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

AABB AABB::findBoundsIfRotated(float rot, const glm::vec3& rotAxis) const
{
	if (OWUtils::isZero(rot))
		return *this;
	glm::mat4 model = glm::rotate(glm::mat4(1), rot, rotAxis);
	return findBoundsIfRotated(model);
}

AABB AABB::findBoundsIfRotated(const glm::mat4& m) const
{
	std::vector<std::vector<glm::vec3>> surfaces = this->surfaces();
	std::vector<glm::vec3> corners;
	for (const std::vector<glm::vec3>& surf : surfaces)
	{
		for (const glm::vec3& pt : surf)
		{
			glm::vec3 pt1 = pt - center();
			glm::vec4 rotatedPoint = m * glm::vec4(pt1, 1);
			pt1 = glm::vec3({ rotatedPoint.x, rotatedPoint.y, rotatedPoint.z }) + center();
			corners.push_back(pt1);
		}
	}
	AABB b2(corners);
	return b2;
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
