#include "OWMovableComponent.h"

#include <glm/gtc/epsilon.hpp>

#include "OWActor.h"
#include "CommonUtils.h"
#include "../Renderers/VAOBuffer.h"
#include "../Helpers/Shader.h"
#include "../Geometry/Ray.h"

static glm::vec3 gZero = glm::vec3(0.0f);

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

bool findIntersectionPoint(const AABB& bounds, const AABB& otherBounds, glm::vec3& intersectionPoint, 
	glm::vec3& myVelocity, const glm::vec3& otherVelocity)
{
	// https://www.rose-hulman.edu/class/cs/csse451/AABB/
	glm::vec3 relativeVelocity = myVelocity - otherVelocity;
	const Compass::Direction dir = bounds.intersectionDirection(otherBounds);
	if (dir == Compass::Direction::NoDirection)
		return false;
	const glm::vec3 halfBoundsSize = (bounds.maxPoint() - bounds.minPoint()) / 2.0f;
	switch (dir)
	{
	case Compass::Direction::North:
		// relativeVelocity.y cannot be zero if we intersect the top plane of the AABB
		intersectionPoint.x = halfBoundsSize.x * (relativeVelocity.x / relativeVelocity.y);
		intersectionPoint.y = bounds.maxPoint().y;
		intersectionPoint.z = halfBoundsSize.z * (relativeVelocity.z / relativeVelocity.y);
		myVelocity.y *= -1;
		break;
	case Compass::Direction::South:
		intersectionPoint.x = halfBoundsSize.x * (relativeVelocity.x / relativeVelocity.y);
		intersectionPoint.y = bounds.minPoint().y;
		intersectionPoint.z = halfBoundsSize.z * (relativeVelocity.z / relativeVelocity.y);
		myVelocity.y *= -1;
		break;
	case Compass::Direction::East:
		intersectionPoint.x = bounds.maxPoint().x;
		intersectionPoint.y = halfBoundsSize.y * (relativeVelocity.y / relativeVelocity.x);
		intersectionPoint.z = halfBoundsSize.z * (relativeVelocity.z / relativeVelocity.x);
		myVelocity.x *= -1;
		break;
	case Compass::Direction::West:
		intersectionPoint.x = bounds.minPoint().x;
		intersectionPoint.y = halfBoundsSize.y * (relativeVelocity.y / relativeVelocity.x);
		intersectionPoint.z = halfBoundsSize.z * (relativeVelocity.z / relativeVelocity.x);
		myVelocity.x *= -1;
		break;
	case Compass::Direction::In:
		intersectionPoint.x = halfBoundsSize.x * (relativeVelocity.x / relativeVelocity.z);
		intersectionPoint.y = halfBoundsSize.y * (relativeVelocity.y / relativeVelocity.z);
		intersectionPoint.z = bounds.maxPoint().z;
		myVelocity.z *= -1;
		break;
	case Compass::Direction::Out:
		intersectionPoint.x = halfBoundsSize.x * (relativeVelocity.x / relativeVelocity.z);
		intersectionPoint.y = halfBoundsSize.y * (relativeVelocity.y / relativeVelocity.z);
		intersectionPoint.z = bounds.minPoint().z;
		myVelocity.z *= -1;
		break;
	default:
		break;
	}
	return true;
}

bool OWMovableComponent::validBoundingBox() const
{
	return !glm::all(glm::epsilonEqual(mBoundBoxOriginal.minPoint(), mBoundBoxOriginal.maxPoint(), OWUtils::epsilon()));
}

VAOBuffer* OWMovableComponent::boundingBoxRenderer()
{
	if (mRenderBoundingBox && mBoundingBoxRenderer == nullptr && validBoundingBox())
	{
		std::vector<std::vector<glm::vec3>> surfaces = mBoundBoxOriginal.surfaces();
		Shader* lineShader = new Shader();
		lineShader->loadBoilerPlates();
		lineShader->setStandardUniformNames("pvm");

		mBoundingBoxRenderer = new VAOBuffer(lineShader, VAOBuffer::DRAW_MULTI);

		for (const std::vector<glm::vec3>& p : surfaces)
		{
			MeshDataLight lineData;
			lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN), "colour");
			lineData.vertices(p, GL_LINE_STRIP);
			mBoundingBoxRenderer->add(&lineData);
		}
		mBoundingBoxRenderer->prepare();
	}
	return mBoundingBoxRenderer;
}

void OWMovableComponent::collided(OWMovableComponent* other)
{
	// https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	glm::vec3 v = mCurrent.mVelocity + other->mCurrent.mVelocity;
	glm::vec3 normal;
	float distance = 0.0f;
	if (glm::all(glm::epsilonEqual(v, glm::vec3(0), OWUtils::epsilon())))
		return;
	if (!Ray::rayIntersectsBox(bounds().center(), v, other->bounds(), normal, distance))
	{
		return;
		// throw NMSLogicException("Object [" + name() + "] collided by ray intersection falied");
	}
	v = glm::normalize(v);
	// https://stackoverflow.com/questions/573084/how-to-calculate-bounce-angle
	glm::vec3 reboundDir  = v - 2 * glm::dot(v, normal) * normal;
	glm::vec3 ourCenter = bounds().center();
	glm::vec3 otherCenter = other->bounds().center();
	float len = glm::length(ourCenter - otherCenter);
	float prorataDistance = (len - distance) / len;
	glm::vec3 positionAfterBounce = mCurrent.mPosition + prorataDistance * glm::normalize(reboundDir);
	mCurrent.mPosition = positionAfterBounce;
	mCurrent.mVelocity = reboundDir * glm::length(mCurrent.mVelocity);
}

void OWMovableComponent::bounds(const AABB& bb)
{
	if (!validBoundingBox())
	{
		mBoundBoxOriginal = bb;
		glm::vec3 mi = bb.minPoint() + position();
		glm::vec3 ma = bb.maxPoint() + position();
		mBoundingBox = AABB(mi, ma);
	}
	else
	{
		mBoundingBox = bb;
	}
}

int OWMovableComponent::tick(float dt)
{
	bool retval = glm::epsilonEqual(dt, 0.0f, OWUtils::epsilon()) &&
		glm::all(glm::epsilonEqual(mCurrent.mVelocity, gZero, OWUtils::epsilon()));
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
