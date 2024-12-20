#pragma once

#include "../OWEngine/OWEngine.h"
#include "OWComponent.h"
#include "../Geometry/BoundingBox.h"

class OWENGINE_API PhysicalTypeMetaData
{
public:
	enum ThreeWay { Yes, No, Maybe };
	glm::vec3 mGravity = glm::vec3(0);
	glm::vec3 mMaxVelosity = glm::vec3(0);
	glm::vec3 mAcceleration = glm::vec3(0);
	float mHardness = 0.0f;
	ThreeWay mVisible = ThreeWay::Maybe;
};

class OWENGINE_API ParticleData
{
public:
	glm::vec3 mPosition = glm::vec3(0);
	glm::vec3 mVelocity = glm::vec3(0);
	glm::vec3 mAcceleration = glm::vec3(0);
	glm::vec3 mOrientation = glm::vec3(0);
	glm::vec3 mRotationVelocity = glm::vec3(0);
	//glm::quat mOrientation = glm::quat();     //orientation in 3D space
	float mMass = 1.0f;                 // measured in Kg
	bool mIsStatic = true;
	PhysicalTypeMetaData::ThreeWay mVisible = PhysicalTypeMetaData::ThreeWay::Maybe;
};

class OWBounding;
class OWENGINE_API OWMovableComponent : public OWComponent
{
	AABB mBoundingBox = AABB(glm::vec3(-1), glm::vec3(-1));
	ParticleData mCurrent;
	ParticleData mPrevious;
	PhysicalTypeMetaData* mClassSpecs = nullptr;
	OWBounding* mFineGrain = nullptr;
	OWBounding* mHitSphere = nullptr;
	glm::vec3 mHitSphereOffset = glm::vec3(0);
	AABB mHitBox;
	glm::vec3 mSteerForce = glm::vec3(0);       //These are all of the forces acting on the object accelleration (thrust, gravity, drag, etc) 
	float mScale = 1.0f;         //this is the scale factor for the object
public:
	OWMovableComponent(OWActor* _owner, const glm::vec3& _position)
		: OWComponent(_owner)
	{
		mCurrent.mPosition = _position;
	}
	const AABB& bounds() const { return mBoundingBox; }
	void bounds(const AABB& bb) { mBoundingBox = bb; }
	const glm::vec3& position() const { return mCurrent.mPosition; }
	void position(const glm::vec3& value) { mCurrent.mPosition = value; }
	int tick(float dt) override; 
	void velocity(const glm::vec3& direction, float speed)
	{
		velocity(glm::normalize(direction) * speed);
	}
	void velocity(const glm::vec3& value)
	{
		// check does not exceed max velocity
		mCurrent.mVelocity = value;
	}
	virtual bool canCollide() { return true; }
	virtual bool canCollide(OWMovableComponent* other);
	virtual bool collides(OWMovableComponent* other);
	virtual void collided(OWMovableComponent* OW_UNUSED(other)) {}
protected:
	virtual bool fineGrainCollide(OWMovableComponent* OW_UNUSED(other))
	{
		return false;
	}
};
