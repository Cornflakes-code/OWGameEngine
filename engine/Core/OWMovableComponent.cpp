#include "OWMovableComponent.h"

#include <glm/gtc/epsilon.hpp>

#include "OWActor.h"
#include "CommonUtils.h"
#include "../Renderers/VAOBuffer.h"
#include "../Helpers/Shader.h"
#include "../Geometry/Ray.h"
#include "../Core/LogStream.h"
#include "../Geometry/OWRay.h"

static glm::vec3 gZero = glm::vec3(0.0f);

bool OWMovableComponent::canCollide(OWMovableComponent* other)
{
	if (other == this)
		return false;
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
	if (!mMoveable)
		return;
	// https://gamedev.stackexchange.com/questions/47888/find-the-contact-normal-of-rectangle-collision?noredirect=1&lq=1
	glm::vec3 relVel = mCurrent.mVelocity - other->mCurrent.mVelocity;
	glm::vec3 compoundSize(bounds().size() + other->bounds().size());
	AABB compoundAABB(compoundSize);
	glm::vec3 jfw = bounds().center();
	compoundAABB.moveTo(bounds().center());
	glm::vec3 normal1, normal2;
	float distance1, distance2;

	OWRay r1(bounds().center(), relVel);
	if (!r1.intersects(bounds(), normal1, distance1))
	{
		return;
	}
	OWRay r31(bounds().center(), relVel);
	if (!r1.intersects(compoundAABB, normal2, distance2))
	{
		return;
	}
	// Various collision detection algorythmns
	// https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-32-broad-phase-collision-detection-cuda
	// https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	glm::vec3 v = mCurrent.mVelocity + other->mCurrent.mVelocity;
	glm::vec3 normal;
	float distance;
	if (glm::all(glm::epsilonEqual(v, glm::vec3(0), OWUtils::epsilon())))
		return;
	glm::vec3 c1 = other->bounds().minPoint() - glm::vec3(2, 2, 2);
	OWRay r2(bounds().center(), v);
	if (!r2.intersects(other->bounds(), normal, distance))
	{
		return;
		// throw NMSLogicException("Object [" + name() + "] collided by ray intersection failed");
	}
	v = glm::normalize(v);
	// https://stackoverflow.com/questions/573084/how-to-calculate-bounce-angle
	// https://3dkingdoms.com/weekly/weekly.php?a=2
	constexpr float notPerfectBounce = 1.0f; // A perfect bounce
	glm::vec3 reboundDir  = notPerfectBounce * (v - 2 * glm::dot(v, normal) * normal);
	glm::vec3 ourCenter = bounds().center();
	glm::vec3 otherCenter = other->bounds().center();
	float dist = glm::length(ourCenter - otherCenter);
	float fullTimeStep = glm::length(previousPosition() - position()) / glm::length(mCurrent.mVelocity);
	float curtailedTimeStep = glm::length(distance) / glm::length(mCurrent.mVelocity);
	// jfw prorataDistance is wrong.
	float len = glm::length(ourCenter - otherCenter);
	float len2 = glm::length(ourCenter - position());
	float prorataTimeStep = distance / len;// curtailedTimeStep / fullTimeStep;
	move(prorataTimeStep * glm::length(mCurrent.mVelocity) * glm::normalize(reboundDir));
	mCurrent.mVelocity = reboundDir * glm::length(mCurrent.mVelocity);
}

void OWMovableComponent::bounds(const AABB& bb)
{
	if (!validBoundingBox())
	{
		mBoundBoxOriginal = bb;
		if (false)
		{
			glm::vec3 mi = bb.minPoint() + position();
			glm::vec3 ma = bb.maxPoint() + position();
			mBoundingBox = AABB(mi, ma);
		}
		else
		{
			glm::vec3 mi = bb.minPoint() + position();
			glm::vec3 ma = bb.maxPoint() + position();
			moveTo(position());
			mBoundingBox = AABB(mi, ma);
		}
	}
	else
	{
		mBoundingBox = bb;
	}
}

void OWMovableComponent::move(const glm::vec3& moveStep)
{
	mCurrent.mPosition += moveStep;
	if (mFineGrain)
		mFineGrain->move(moveStep);
	if (mHitSphere)
		mHitSphere->move(moveStep);
	mBoundingBox.move(moveStep);
	//mHitBox.move(moveStep);
}

void OWMovableComponent::moveTo(const glm::vec3& newPosition)
{
	mCurrent.mPosition = newPosition;
	if (mFineGrain)
		mFineGrain->moveTo(newPosition);
	if (mHitSphere)
		mHitSphere->moveTo(newPosition);
	mBoundingBox.moveTo(newPosition);
	//mHitBox.moveTo(newPosition);
}

int OWMovableComponent::tick(float dt)
{
	bool retval = glm::epsilonEqual(dt, 0.0f, OWUtils::epsilon()) &&
		glm::all(glm::epsilonEqual(mCurrent.mVelocity, gZero, OWUtils::epsilon()));
	mPrevious = mCurrent;
	glm::vec3 moveStep = mCurrent.mVelocity * dt;
	move(moveStep);
	mCurrent.mVelocity += mCurrent.mAcceleration * dt;
	return retval? 1: 0;
}
