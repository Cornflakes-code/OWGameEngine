#include "MoveController.h"
#include <limits>

#include <glm/ext/matrix_transform.hpp>

#include "../Core/ErrorHandling.h"

MoveController::MoveController()
{
}

void MoveController::targetGeometry(const AABB& targetBounds, 
									const glm::vec3& initialPosition)
{ 
	mBounds = targetBounds;
	mInitialPosition = initialPosition;

	// Start the minPoint of the text at 0,0,0.
	setPosition(initialPosition);
	mBounds.move(mBounds.minPoint());
}

void MoveController::move(const glm::vec3& speed)
{
	mBounds.move(mDirection * speed);
}

void MoveController::setPosition(const glm::vec3& newValue)
{
	mBounds.move(-mBounds.minPoint() + newValue);
}

/*
Compass::Direction wallIntersection(const AABB& box, const AABB& other)
{
	if (!box.intersects(other))
		return Compass::Direction::NoDirection;
	if (other.maxPoint().y >= box.maxPoint().y)
		return Compass::Direction::North;
	else if (other.maxPoint().x >= box.maxPoint().x)
		return Compass::Direction::East;
	else if (other.maxPoint().z >= box.maxPoint().z)
		return Compass::Direction::In;
	else if (other.minPoint().y <= box.minPoint().y)
		return Compass::Direction::South;
	else if (other.minPoint().x <= box.minPoint().x)
		return Compass::Direction::West;
	else if (other.minPoint().z <= box.minPoint().z)
		return Compass::Direction::Out;
	return Compass::Direction::NoDirection;
}
*/

Compass::Direction MoveController::wallIntersection(const AABB& scenery)
{
	return scenery.intersectionDirection(mBounds);
}

void MoveController::bounceIfCollide(const AABB& scenery)
{
	Compass::Direction dir = wallIntersection(scenery);

	// .. and, if collided, then adjust for bounce
	if (dir < Compass::Direction::NoDirection)
	{
		// If we pass the boundary then make the Bounding Box 'bounce'
		glm::vec4 rebound(0.0);
		switch (dir)
		{
		case Compass::Direction::North:
			rebound.y = 2 * (scenery.maxPoint().y - mBounds.maxPoint().y);
			mDirection.y = -mDirection.y;
			break;
		case Compass::Direction::South:
			rebound.y = 2 * (scenery.minPoint().y - mBounds.minPoint().y);
			mDirection.y = -mDirection.y;
			break;
		case Compass::Direction::East:
			rebound.x = 2 * (scenery.maxPoint().x - mBounds.maxPoint().x);
			mDirection.x = -mDirection.x;
			break;
		case Compass::Direction::West:
			rebound.x = 2 * (scenery.minPoint().x - mBounds.minPoint().x);
			mDirection.x = -mDirection.x;
			break;
		case Compass::Direction::In:
			rebound.z = 2 * (scenery.maxPoint().z - mBounds.maxPoint().z);
			mDirection.z = -mDirection.z;
			break;
		case Compass::Direction::Out:
			rebound.z = 2 * (scenery.minPoint().z - mBounds.minPoint().z);
			mDirection.z = -mDirection.z;
			break;
		default:
			break;
		}
		mBounds.move(rebound);
	}
}
