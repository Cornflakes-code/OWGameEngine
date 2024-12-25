#include "OWMovableComponent.h"

#include <glm/gtc/epsilon.hpp>
#include "OWActor.h"

bool OWMovableComponent::canCollide(OWMovableComponent* other)
{
	// called before testing for couldCollide
	return mOwner->canCollide(this, other->mOwner, other);
}

bool OWMovableComponent::collides(OWMovableComponent* other)
{
	// if this is called then we know that AABB's of these objects intersect.
	if (fineGrainCollide(other))
	{
		// We really did collide
		// Allow the actors to do something
		if (!mOwner->collideHandled(this, other->mOwner, other))
		{
			// here we update positions
		}
	}
	return true;
}

static constexpr float gEpsilon = 0.0001f;
static glm::vec3 gZero = glm::vec3(0.0f);

int OWMovableComponent::tick(float dt)
{
	bool retval = glm::epsilonEqual(dt, 0.0f, 0.0000000f) &&
		glm::all(glm::epsilonEqual(mCurrent.mVelocity, gZero, gEpsilon));
	mPrevious = mCurrent;
	glm::vec3 moveStep = mCurrent.mVelocity * dt;
	mCurrent.mPosition += moveStep;
	if (mFineGrain)
		mFineGrain->move(moveStep);
	if (mHitSphere)
		mHitSphere->move(moveStep);
	mBoundingBox.move(moveStep);
	//mHitBox.move(moveStep);

	mCurrent.mVelocity += mCurrent.mAcceleration * dt;
	return retval? 1: 0;
}
