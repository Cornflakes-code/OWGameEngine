#include "Collider.h"

#include "../Core/ErrorHandling.h"

glm::vec3 OWCollider::bounds(bool min) const
{
	return min ? mPt1 : mPt2;
}

glm::vec3 OWCollider::left() const
{
	switch (mColliderType)
	{
	case OWCollider::CollisionType::Ovoid:
		throw NMSLogicException("Error: OWCollider(Ovoid)::left not implemented.");
		break;
	case OWCollider::CollisionType::Box:
		return mPt1 - (mPt2 / 2.0f);
		break;
	case OWCollider::CollisionType::Plane:
		return mPt1 - mPt2;
		break;
	case OWCollider::CollisionType::Point:
		return mPt1;
		break;
	case OWCollider::CollisionType::Ray:
		return glm::min(mPt1, mPt1 + mPt2);
		break;
	default:
		throw NMSLogicException("Error: OWCollider(Ovoid)::left unknown collider type.");
		break;
	}
}

glm::vec3 OWCollider::right() const
{
	switch (mColliderType)
	{
	case OWCollider::CollisionType::Ovoid:
		throw NMSLogicException("Error: OWCollider(Ovoid)::right not implemented.");
		break;
	case OWCollider::CollisionType::Box:
		return mPt1 + (mPt2 / 2.0f);
		break;
	case OWCollider::CollisionType::Plane:
		return mPt1 + mPt3;
		break;
	case OWCollider::CollisionType::Point:
		return mPt1;
		break;
	case OWCollider::CollisionType::Ray:
		return glm::max(mPt1, mPt1 + mPt2);
		break;
	default:
		throw NMSLogicException("Error: OWCollider(Ovoid)::right unknown collider type.");
		break;
	}
}

bool OWCollider::collides(const OWCollider& other) const
{
	if (actor() == other.actor())
		return false;
	// jfw fix me
	return false;
}

void OWCollider::points(const AABB& bounds)
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
		mPt1 = bounds.minPoint();
		mPt2 = bounds.maxPoint();
		break;
	case OWCollider::CollisionType::Point:
		mPt1 = bounds.center();
		mPt2 = glm::vec3(bounds.diagonal());
		break;
	case OWCollider::CollisionType::Ray:
		mPt1 = bounds.minPoint();
		mPt2 = bounds.maxPoint();
		break;
	default:
		break;
	}
}
