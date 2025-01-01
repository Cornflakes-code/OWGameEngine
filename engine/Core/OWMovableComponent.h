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
class VAOBuffer;
class OWENGINE_API OWMovableComponent : public OWComponent
{
	// We need an up to date mBoundingBox for collision calculation purposes.
	// For correct scaling while rendering we need to compare against the original.
	AABB mBoundingBox = AABB(glm::vec3(-1), glm::vec3(-1));
	AABB mBoundBoxOriginal = AABB(glm::vec3(-1), glm::vec3(-1));
	ParticleData mCurrent;
	ParticleData mPrevious;
	PhysicalTypeMetaData* mClassSpecs = nullptr;
	OWBounding* mFineGrain = nullptr;
	OWBounding* mHitSphere = nullptr;
	glm::vec3 mHitSphereOffset = glm::vec3(0);
	AABB mHitBox;
	glm::vec3 mSteerForce = glm::vec3(0);       //These are all of the forces acting on the object accelleration (thrust, gravity, drag, etc) 
	VAOBuffer* mBoundingBoxRenderer = nullptr;
	bool validBoundingBox() const;
	void move(const glm::vec3& moveStep);
	void moveTo(const glm::vec3& moveStep);
protected:
	VAOBuffer* boundingBoxRenderer();
	const AABB& boundBoxOriginal() const 
	{
		return mBoundBoxOriginal;
	}
	bool mRenderBoundingBox = true;
public:
	OWMovableComponent(OWActor* _owner, const glm::vec3& _position)
		: OWComponent(_owner)
	{
		position(_position);
	}
	void renderBoundingBox(bool _value) { mRenderBoundingBox = _value; }
	const AABB& bounds() const 
	{ 
		return mBoundingBox; 
	}
	void bounds(const AABB& bb);
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
	virtual void collided(OWMovableComponent* other);
protected:
	virtual bool fineGrainCollide(OWMovableComponent* OW_UNUSED(other))
	{
		return false;
	}
};
