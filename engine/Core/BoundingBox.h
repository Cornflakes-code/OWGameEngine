#pragma once

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "ErrorHandling.h"

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
};

namespace Compass
{
	enum Direction
	{
		North,
		South,
		East,
		West,
		In,
		Out,
		NoDirection,
		NumDirections
	};
	std::string OWENGINE_API asString(Direction dir);
	extern glm::vec4 OWENGINE_API Rose[NumDirections];
};

// And thanks to https://gist.github.com/podgorskiy/308f0438a77ddee78cc12148ee654e95
// for some good ideas.
template <int Dim, typename Type>
class OWENGINE_API AABB
{
	static constexpr float _max = std::numeric_limits<float>::max();
	static constexpr float _epsilon = 0.0001f;
public:
	typedef glm::vec<Dim, Type> vec_type;
	AABB<Dim, Type>()
		: mMinPoint(vec_type(_max)), mMaxPoint(vec_type(-_max))
	{}

	AABB<Dim, Type>(const vec_type& _minPoint, const vec_type& _maxPoint, bool validate = true)
	: mMinPoint(_minPoint), mMaxPoint(_maxPoint)
	{
		if (validate)
			isValid();
	}

	AABB<Dim, Type>(const std::vector<vec_type>& v)
	{
		// Note: No depth value: default to 0;
		mMinPoint = vec_type(_max);
		mMaxPoint = vec_type(-_max);

		for (const vec_type& point : v)
		{
			mMinPoint = glm::min(point, mMinPoint);
			mMaxPoint = glm::max(point, mMaxPoint);
		}
	}
	AABB<Dim, Type>(const std::vector<AABB<Dim, Type>>& v)
	{
		mMinPoint = vec_type(_max);
		mMaxPoint = vec_type(-_max);
		for (const AABB& ab : v)
		{
			mMinPoint = glm::min(ab.minPoint(), mMinPoint);
			mMaxPoint = glm::max(ab.maxPoint(), mMaxPoint);
		}
	}

	bool intersects(const AABB<Dim, Type>& other) const
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

	Compass::Direction intersectionDirection(const AABB<Dim, Type>& other) const
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

	// If the point is inside or on the border of the aabb returns true, otherwise false
	inline bool inside(AABB<Dim, Type> x, const glm::vec<Dim, Type> point) const
	{
		return	glm::all(glm::lessThanEqual(x.minp, point)) && 
				glm::all(glm::greaterThanEqual(x.maxp, point));
	}	
	
	vec_type size() const { return mMaxPoint - mMinPoint; }

	const vec_type minPoint() const { return mMinPoint; }
	const vec_type maxPoint() const { return mMaxPoint; }
	void move(const vec_type& moveBy)
	{
		mMinPoint += moveBy;
		mMaxPoint += moveBy;
	}
	vec_type center() const
	{ 
		vec_type halfSize = size();
		halfSize /= 2.0f;
		return mMinPoint + halfSize;
	}
	void isValid() const
	{
		if (mMinPoint.x > mMaxPoint.x || mMinPoint.y > mMaxPoint.y || mMinPoint.z > mMaxPoint.z)
			throw NMSLogicException("Error: Invalid AABB size.");
	}
	bool operator==(const AABB<Dim, Type>& other) const
	{
		return glm::all(glm::epsilonEqual(mMinPoint, other.mMinPoint, _epsilon))
			&& glm::all(glm::epsilonEqual(mMaxPoint, other.mMaxPoint, _epsilon));
	}
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	vec_type mMinPoint;
	vec_type mMaxPoint;
#pragma warning( pop )
};

template <int Dim, typename Type>
inline AABB<Dim, Type> operator | (const AABB<Dim, Type>& x, const glm::vec<Dim, Type>& point)
{
	return AABB<Dim, Type>(min(x.minPoint(), point), max(x.maxPoint(), point));
}
// Union of a point with aabb
template <int Dim, typename Type>
inline AABB<Dim, Type> operator |= (AABB<Dim, Type>& x, const glm::vec<Dim, Type>& point)
{
	x = x | point;
	return x;
}

// Union of two aabb's
template <int Dim, typename Type>
inline AABB<Dim, Type> operator | (const AABB<Dim, Type>& a, const AABB<Dim, Type>& b)
{
	return (a | b.minPoint()) | b.maxPoint();
}

// Union of two aabb's
template <int Dim, typename Type>
inline AABB<Dim, Type> operator |= (AABB<Dim, Type>& a, const AABB<Dim, Type>& b)
{
	a = a | b;
	return a;
}

// Intersection of two aabb's
template <int Dim, typename Type>
inline AABB<Dim, Type> operator & (const AABB<Dim, Type>& a, const AABB<Dim, Type>& b)
{
	return AABB<Dim, Type>(max(a.minPoint(), b.minPoint()), min(a.maxPoint(), b.maxPoint()));
}

// Intersection of two aabb's
template <int Dim, typename Type>
inline AABB<Dim, Type> operator &= (AABB<Dim, Type>& a, const AABB<Dim, Type>& b)
{
	a = a & b;
	return a;
}

typedef AABB<2, float> AABBV2;
typedef AABB<3, float> AABBV3;
typedef AABB<4, float> AABBV4;
AABBV4 convertToV4(const AABBV3& v3);
AABBV3 convertToV3(const AABBV4& v4);


