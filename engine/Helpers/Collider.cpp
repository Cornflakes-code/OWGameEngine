#include "Collider.h"

glm::vec2 OWCollider::bounds(bool min) const
{
	throw NMSNotYetImplementedException("CollionSystem::preTick()");
	return glm::vec3(0);
}

bool OWCollider::collides(const OWCollider& other) const
{
	if (actor() == other.actor())
		return false;
	return true;
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
		throw NMSLogicException("Error: OWCollider(Ray) cannot infer pt3 from bounds.");
		break;
	default:
		break;
	}
}
