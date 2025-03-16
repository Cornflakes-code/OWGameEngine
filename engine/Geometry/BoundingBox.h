#pragma once

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"
#include "../Core/ErrorHandling.h"
#include "../Core/CommonUtils.h"
#include "OWBounding.h"

// https://en.wikibooks.org/wiki/OpenGL_Programming/Bounding_box

//	Mainly provides a AABB box for early functionality. Will morph into a more 
//	sophisticated implementation


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

class OWENGINE_API Range
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec3 mMinimum;
	glm::vec3 mCurrent;
	glm::vec3 mMaximum;
#pragma warning( pop )
public:
	Range()
		:Range(-1, 1, 0)  {}

	Range(float _min, float _max, float _curr)
		: mMinimum(_min), mMaximum(_max), mCurrent(_curr)
	{		
	}
	const glm::vec3& maximum() const { return mMaximum; }
	const glm::vec3& current() const { return mCurrent; }
	const void maximum(const glm::vec3& _value) { mMaximum = _value; }
	const void minimum(const glm::vec3& _value) { mMinimum = _value; }

	void addTo(const glm::vec3& toAdd)
	{
		current(current() + toAdd);
	}
	void increaseBy(float scale)
	{
		current(current() * scale);
	}

	void current(const glm::vec3& _curr) 
	{
		float temp = glm::length(_curr);
		if (temp > glm::length(mMaximum))
			mCurrent = mMaximum;
		else if (temp < glm::length(mMinimum))
			mCurrent = mMinimum;
		else mCurrent = _curr;
	}
	float length() const
	{
		return glm::length(mCurrent);
	}
};

class OWENGINE_API AABB: public OWBounding
{
	static constexpr float _max = std::numeric_limits<float>::max();
public:
	AABB()
		: mMinPoint(glm::vec3(_max)), mMaxPoint(glm::vec3(-_max))
	{}

	AABB(float _value)
		: mMinPoint(glm::vec3(_value)), mMaxPoint(glm::vec3(-_value))
	{}
	AABB(const glm::vec3& _minPoint, const glm::vec3& _maxPoint, bool validate = true)
	: mMinPoint(_minPoint), mMaxPoint(_maxPoint)
	{
		if (validate)
			isValid();
	}
	AABB(const glm::vec3& size);
	AABB(const std::vector<glm::vec3>& v)
		: AABB(v.begin(), v.end()) 
	{}
	AABB(std::vector<glm::vec3>::const_iterator begin, std::vector<glm::vec3>::const_iterator end);
	AABB(const std::vector<glm::vec4>& v);
	AABB(const std::vector<AABB>& v);
	float diagonal() const;
	glm::vec3 size() const { return mMaxPoint - mMinPoint; }
	const glm::vec3 minPoint() const { return mMinPoint; }
	const glm::vec3 maxPoint() const { return mMaxPoint; }

	void scale(const glm::vec3& factor);
	bool intersects(const OWBounding* other) const override;
	bool intersects(const AABB& other) const;

	bool contains(const glm::vec3& pt) const
	{
		return (pt.x > mMinPoint.x && pt.x < mMaxPoint.x &&
				pt.y > mMinPoint.y && pt.y < mMaxPoint.y &&
				pt.z > mMinPoint.z && pt.z < mMaxPoint.z);
	}
	// If the point is inside or on the border of the aabb returns true, otherwise false
	inline bool inside(const glm::vec3& point) const
	{
		return	glm::all(glm::lessThanEqual(mMinPoint, point)) && 
				glm::all(glm::greaterThanEqual(mMaxPoint, point));
	}	

	// We may need to a new AABB that fits this if this was rotated.
	AABB findBoundsIfRotated(float rot, const glm::vec3& rotAxis) const;
	AABB findBoundsIfRotated(const glm::mat4& m) const;

	void move(const glm::vec3& delta) override
	{
		mMinPoint += delta;
		mMaxPoint += delta;
	}
	void move(const glm::vec4& delta) override
	{
		move(glm::vec3(delta));
	}

	void moveTo(const glm::vec3& pt) override
	{
		glm::vec3 c = center();
		mMinPoint = mMinPoint - c + pt;
		mMaxPoint = mMaxPoint - c + pt;
	}

	glm::vec3 center() const
	{
		glm::vec3 halfSize = size() / 2.0f;
		return mMinPoint + halfSize;
	}

	// 6 vectors of a surfaces four points. The points are clockwise as viewed from the center
	std::vector<std::vector<glm::vec3>> surfaces() const;
	void isValid() const
	{
		if (mMinPoint.x > mMaxPoint.x || mMinPoint.y > mMaxPoint.y || mMinPoint.z > mMaxPoint.z)
			throw NMSLogicException("Error: Invalid AABB size.");
	}
	bool operator==(const AABB& other) const
	{
		return glm::all(glm::epsilonEqual(mMinPoint, other.mMinPoint, OWUtils::epsilon()))
			&& glm::all(glm::epsilonEqual(mMaxPoint, other.mMaxPoint, OWUtils::epsilon()));
	}
	glm::vec3 extent() const 
	{
		return (mMaxPoint - mMinPoint) / 2.0f;
	}
	bool render() const { return mRender; }
	void render(bool newValue) { mRender = newValue; }
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec3 mMinPoint;
	glm::vec3 mMaxPoint;
	bool mRender = false;
#pragma warning( pop )
};

inline AABB operator | (const AABB& x, const glm::vec3& point)
{
	return AABB(min(x.minPoint(), point), max(x.maxPoint(), point));
}
// Union of a point with aabb
inline AABB operator |= (AABB& x, const glm::vec3& point)
{
	x = x | point;
	return x;
}

// Union of two aabb's
inline AABB operator | (const AABB& a, const AABB& b)
{
	return (a | b.minPoint()) | b.maxPoint();
}

// Union of two aabb's
inline AABB operator |= (AABB& a, const AABB& b)
{
	a = a | b;
	return a;
}

// Intersection of two aabb's
inline AABB operator & (const AABB& a, const AABB& b)
{
	return AABB(max(a.minPoint(), b.minPoint()), min(a.maxPoint(), b.maxPoint()));
}

// Intersection of two aabb's
inline AABB operator &= (AABB& a, const AABB& b)
{
	a = a & b;
	return a;
}

std::vector<glm::vec3> convertToV3(const std::vector<glm::vec4>& v4);
