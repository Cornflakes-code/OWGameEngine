#include "MovingText.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Helpers/TextBillboard.h"
#include "../Helpers/Shader.h"

#include "../Renderables/Triangle.h"

MovingText::MovingText()
{
	mTriangle = new Triangle(nullptr);
	mTriangle->setUp();
}

void MovingText::fixedTimeStep(float seconds)
{

}

Compass::Direction MovingText::wallIntersection(const AABB& scenery)
{
	return scenery.wallIntersection(mBounds);
}

void MovingText::text(TextBillboard* newValue)
{ 
	mBounds = newValue->bounds();
	mText.setUp(newValue);

	// Start the minPoint of the text at 0,0,0.
	mBounds = mBounds - mBounds.minPoint();
}

void MovingText::bounceIfCollide(const AABB& scenery)
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
		mTriangle->move(rebound);
	}
}

void MovingText::setPosition(const glm::vec3& newValue)
{
	// move to origin and then position
	mBounds = mBounds - mBounds.minPoint() + newValue;
	mTriangle->setPosition(newValue);
}

void MovingText::move(const glm::vec4& velocity)
{
	mBounds.move(mDirection * velocity);
	mTriangle->move(mDirection * velocity);
}

void MovingText::render(const glm::mat4& proj, 
						const glm::mat4& view,
						const glm::mat4& model) const
{
	glm::mat4 temp = glm::translate(model, glm::vec3(mBounds.minPoint().x, 
													 mBounds.minPoint().y, 
													 mBounds.minPoint().z));
	
	mText.render(proj, view, temp);
	mTriangle->render(proj, view, model);
}
