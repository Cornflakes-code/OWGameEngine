#pragma once

#include <glm/glm.hpp>
#include "../Geometry/BoundingBox.h"

class OWActor;
class OWCollider
{
public:
	enum class CollisionType
	{
		Ovoid, Box, Plane, Point, Ray, Permeable
	};
	OWCollider(OWActor* _actor, CollisionType colliderType, unsigned int _componentIndex = 0)
		: mActor(_actor), mColliderType(colliderType), mComponentIndex(_componentIndex)
	{
	}
	
	CollisionType collisionType() const {
		return mColliderType;
	}

	glm::vec3 left() const;
	glm::vec3 right() const;
	glm::vec3 bounds(bool min) const;
	bool collides(const OWCollider& other) const;
	const OWActor* actor() const { return mActor; }
	OWActor* actor() { return mActor; }
	void actor(OWActor* newValue) { mActor = newValue; }
	void componentIndex(size_t newValue)
	{
		mComponentIndex = static_cast<unsigned int>(newValue);
	}
	size_t componentIndex() const
	{
		return mComponentIndex;
	}
	/*
	* If Ovoid then p1 is center, p2 is radius
	* If Box then p1 is center and p2 is span
	* If Point then p1 is the position and p2 is a constant radius
	* If plane then p1 is a point on the plane, and p2 is minPoint corner relative to 
		p1 and p3 is maxpoint corner relative to p1
	* If Ray then mPt1 is origin, mPt2 is direction, mPt3 is end of ray??
	*/
	void points(const AABB& bounds);
	void position(const glm::vec3& pos) 
	{
		mPt1 = pos;
	}
	void points(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
	{
		mPt1 = p1;
		mPt2 = p2;
		mPt3 = p3;
	}
	friend bool operator==(const OWCollider& lhs, const OWCollider& rhs)
	{
		return lhs.mActor == rhs.mActor && lhs.mComponentIndex == rhs.mComponentIndex;
	}
private:
	glm::vec3 mPt1 = glm::vec3(0);
	glm::vec3 mPt2 = glm::vec3(0);
	glm::vec3 mPt3 = glm::vec3(0);
	OWActor* mActor;
	unsigned int mComponentIndex;
	CollisionType mColliderType = CollisionType::Ovoid;
};