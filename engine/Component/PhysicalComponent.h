#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "../OWEngine/OWEngine.h"
#include "OWComponent.h"
#include "../Geometry/BoundingBox.h"

namespace OWPhysicalDataMaximums
{
	extern glm::vec3 gravity;
	extern glm::vec3 elocity;
	extern glm::vec3 acceleration;
	extern float mass;
	extern float hardness;
};

struct OWPhysicsData
{
	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 rotationalVelocity = glm::vec3(0);
	glm::vec3 acceleration = glm::vec3(0);
	// 0(invisibility -> 1 (fully opaque)
	float visibility = 1.0f;
	float mass = 1.0f;
	float hardness = 0.5f;
};

class OWPhysics
{
	OWPhysicsData mData;
public:
	OWPhysics(const OWPhysicsData& _data = OWPhysicsData())
		: mData(_data)
	{
	}
};

namespace OWPhysicalMetaData
{
	extern glm::vec3 gravity;
	extern glm::vec3 maxVelocity;
	extern glm::vec3 maxAcceleration;
	extern float maxMass;
	extern float hardness;
	enum ChangeType { increment, absolute };
};
class OLDIPhysical;
struct OWENGINE_API OLDCollisionData
{
	AABB boundingBox = AABB(glm::vec3(-1), glm::vec3(-1));
	OLDIPhysical* component = nullptr;
	bool canMove = true;
	bool canCollide = true;
};

struct OWENGINE_API OLDPhysicsDataImp
{
	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 acceleration = glm::vec3(0);
	glm::vec3 orientation = glm::vec3(0);
	glm::vec3 rotationVelocity = glm::vec3(0);
	//glm::quat mOrientation = glm::quat();
	float mass = 1.0f;
	// 0(invisibility -> 1 (fully opaque)
	float visibility = 1.0f;
	glm::vec3 steerForce = glm::vec3(0); // These are all of the forces acting on the object accelleration (thrust, gravity, drag, etc) 
	void translate(const glm::vec3& newValue);
	void rotate(float rads, const glm::vec3& axis);
	void scale(const glm::vec3& newValue);
	void scale(float newValue)
	{
		scale(glm::vec3(newValue));
	}
	glm::vec3 scale() const
	{
		return mScale;
	}
	glm::vec3 mRotate = glm::vec3(0);
	glm::vec3 mTranslate = glm::vec3(0);
	glm::vec3 mScale = glm::vec3(1);
private:
};

struct OWENGINE_API OLDPhysicsData: public OLDCollisionData
{
	OLDPhysicsDataImp physics;
};

class OWENGINE_API OLDIPhysical
{
	OLDPhysicsData* mData = nullptr;
protected:
public:
	glm::vec3 velocity()
	{
		return mData->physics.velocity;
	}

	void velocity(const glm::vec3& newValue)
	{
		mData->physics.velocity = newValue;
	}

	glm::vec3 acceleration()
	{
		return mData->physics.acceleration;
	}

	void acceleration(const glm::vec3& newValue)
	{
		mData->physics.acceleration = newValue;
	}

	glm::vec3 orientation()
	{
		return mData->physics.orientation;
	}

	void orientation(const glm::vec3& newValue)
	{
		mData->physics.orientation = newValue;
	}

	glm::vec3 rotationVelocity()
	{
		return mData->physics.rotationVelocity;
	}
	void rotationVelocity(const glm::vec3& newValue)
	{
		mData->physics.rotationVelocity = newValue;
	}

	float mass()
	{
		return mData->physics.mass;
	}

	void mass(float newValue)
	{
		mData->physics.mass = newValue;
	}

	float visibility()
	{
		return mData->physics.visibility;
	}

	void visibility(float newValue)
	{
		if (newValue < 0.0f)
			newValue = 0.0f;
		else if (newValue > 1.0f)
			newValue = 1.0f;
		mData->physics.visibility = newValue;
	}

	void rotate(float radians, const glm::vec3& axis);
	void scale(const glm::vec3& v);
	void translate(const glm::vec3& v);
	glm::vec3 translation() const {
		return mData->physics.mTranslate;
	}
	glm::vec3 scale() const;
	virtual OLDPhysicsData* data() { return mData; }
	virtual const OLDPhysicsData* constData() const { return mData; }

	// OWCollisionData has a smaller footprint than OLDPhysicsData.
	// Use it for the collision engine
	OLDCollisionData* collisionData() { return mData; }
	void setData(OLDPhysicsData* newValue);
	virtual bool canCollide() = 0;
	virtual bool canCollide(OLDCollisionData* other) = 0;
	virtual bool collides(OLDCollisionData* other) = 0;
	virtual void collided(OLDCollisionData* other) = 0;
	void physicalDoInit();
};
