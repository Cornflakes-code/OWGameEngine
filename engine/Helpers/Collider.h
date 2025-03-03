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
	OWCollider(OWActor* _owner, CollisionType colliderType, unsigned int _componentIndex = 0)
		: mOwner(_owner), mColliderType(colliderType), mComponentIndex(_componentIndex)
	{
	}
	CollisionType collisionType() const {
		return mColliderType;
	}

	void componentIndex(unsigned int newValue) {
		mComponentIndex = newValue;
	}
	/*
	* If Ovoid then p1 is center, p2 is radius
	* If Box then p1 is minPoint and p2 is maxPoint
	* If Point then p1 is the point and p2 is a constant radius
	* If plane then p1 is minPoint and p2 is maxPoint and p3 
	is a constant distance from the plane
	*/
	void points(const AABB& bounds)
	{
		switch (mColliderType)
		{
		case OWCollider::CollisionType::Ovoid:
			mPt1 = bounds.center();
			mPt2 = bounds.size();
			break;
		case OWCollider::CollisionType::Box:
			mPt1 = bounds.minPoint();
			mPt2 = bounds.maxPoint();
			break;
		case OWCollider::CollisionType::Plane:
			throw NMSLogicException("Error: OWCollider(Plane) cannot infer pt3 from bounds.");
			break;
		case OWCollider::CollisionType::Point:
			mPt1 = bounds.center();
			mPt2 = glm::vec3(bounds.diagonal());
			break;
		case OWCollider::CollisionType::Ray:
			throw NMSLogicException("Error: OWCollider(Ray) cannot infer pt3 from bounds.");
			break;
		default:
			break;
		}
	}
	void points(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
	{
		mPt1 = p1;
		mPt2 = p2;
		mPt3 = p3;
	}
private:
	/*
	* If Ovoid then p1 is center, p2 is radius
	* If Box then p1 is minPoint and p2 is maxPoint
	* If Point then p1 is the point
	* If plane then p1, p2, p3 define the plane
	* If Ray then mPt1 is origin, mPt2 is direction, mPt3 is end of ray??
	*/
	glm::vec3 mPt1 = glm::vec3(0);
	glm::vec3 mPt2 = glm::vec3(0);
	glm::vec3 mPt3 = glm::vec3(0);
	OWActor* mOwner;
	unsigned int mComponentIndex;
	CollisionType mColliderType = CollisionType::Ovoid;
};