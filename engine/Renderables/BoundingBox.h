#pragma once

#include <algorithm>
#include "../OWEngine/OWEngine.h"

#include "../OWEngine/OWEngine.h"

#include "../Helpers/CommonUtils.h"

// https://en.wikibooks.org/wiki/OpenGL_Programming/Bounding_box
/*
	Mainly provides a AABB box for early functionality. Will morph into a more 
	sophisticated implementation
*/
class OWENGINE_API Plane
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec4 mPoint1;
	glm::vec4 mPoint2;
	glm::vec4 mPoint3;
	glm::vec4 mNormal;
#pragma warning( pop )
public:
	glm::vec4 ClosestPointOnPlane(const glm::vec4& p);
};

class OWENGINE_API AABB
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec4 mMinPoint;
	glm::vec4 mMaxPoint;
	
	static OWUtils::Float clamp(OWUtils::Float value, OWUtils::Float min, OWUtils::Float max)
	{
		return std::max(min, std::min(max, value));
	}

	static glm::vec4 clamp(const glm::vec4& value, const glm::vec4& _min, const glm::vec4& _max)
	{
		OWUtils::Float x = clamp(value.x, _min.x, _max.x);
		OWUtils::Float y = clamp(value.y, _min.y, _max.y);
		OWUtils::Float z = clamp(value.z, _min.z, _max.z);
		return glm::vec4(x, y, z, 1.0);
	}
	void isValid() const;
#pragma warning( pop )
public:
	AABB(const glm::vec4& _minPoint, const glm::vec4& _maxPoint);
	AABB();
	bool overlap(const AABB& other) const;
	Compass::Direction wallIntersection(const AABB& other) const;
	glm::vec4 size() const { return mMaxPoint - mMinPoint; }

	const glm::vec4 minPoint() const { return mMinPoint; }
	const glm::vec4 maxPoint() const { return mMaxPoint; }
	void minPoint(const glm::vec4& _minPoint) { mMinPoint = _minPoint; }
	void maxPoint(const glm::vec4& _maxPoint) { mMaxPoint = _maxPoint; }
	void move(const glm::vec4& moveBy)
	{
		mMinPoint += moveBy;
		mMaxPoint += moveBy;
	}
	glm::vec4 center() const
	{ 
		glm::vec4 halfSize = size();
		halfSize /= 2.0f;
		return mMinPoint + halfSize;
	}
	// A line is moving from origin to dest. Calc where it ends 
	// up if it bounces off a wall. If no bounce then return dest
	glm::vec4 calcBouncePosition(const glm::vec4& start, const glm::vec4& dest, 
				Compass::Direction whichWall);
		
	// return a point on the side
	glm::vec4 pointAndNormal(Compass::Direction dir, glm::vec4& normal);
	AABB operator+(const glm::vec4& v)
	{
		mMinPoint += v;
		mMaxPoint += v;
		return *this;
	}
	AABB operator-(const glm::vec4& v)
	{
		return *this + -v;
	}
	AABB operator+(const glm::vec3& v)
	{
		return *this + glm::vec4(v, 1.0);
	}
	AABB operator-(const glm::vec3& v)
	{
		return *this + (-glm::vec4(v, 1.0));
	}
};
// These operators do not make much sense
//AABB operator-(const AABB& left, const AABB& right);
//AABB operator+(const AABB& left, const AABB& right);

class OWENGINE_API Ball
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	OWUtils::Float mRadius;
	// Center is topLeft of a box around the ball
	glm::vec4 mPosition;
#pragma warning( pop )
public:
	Ball(const OWUtils::Float& _radius, const glm::vec4& _pos)
	: mRadius(_radius), mPosition(_pos)
	{}
	bool contains(const AABB& other) const;
	glm::vec4 center() const { return mPosition + glm::vec4(mRadius); }
};