#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "../OWEngine/OWEngine.h"
#include "OWComponent.h"
#include "../Geometry/BoundingBox.h"

namespace OWPhysicalMetaData
{
	extern glm::vec3 gravity;
	extern glm::vec3 maxVelocity;
	extern glm::vec3 maxAcceleration;
	extern float maxMass;
	extern float hardness;
	enum ChangeType { increment, absolute };
};

class OWIPhysical;
struct OWENGINE_API OWCollisionData
{
	AABB boundingBox = AABB(glm::vec3(-1), glm::vec3(-1));
	OWIPhysical* component = nullptr;
	bool canMove = true;
	bool canCollide = true;
};

struct OWENGINE_API OWPhysicsDataImp
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
	glm::mat4 localMatrix = glm::mat4(1.0f);
private:
};

struct OWENGINE_API OWPhysicsData: public OWCollisionData
{
	OWPhysicsDataImp physics;
};

class OWENGINE_API OWIPhysical
{
	OWPhysicsData* mData = nullptr;
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
	glm::vec3 scale() const;
	virtual OWPhysicsData* data() { return mData; }
	virtual const OWPhysicsData* constData() const { return mData; }

	// OWCollisionData has a smaller footprint than OWPhysicsData.
	// Use it for the collision engine
	OWCollisionData* collisionData() { return mData; }
	void setData(OWPhysicsData* newValue);
	virtual bool canCollide() = 0;
	virtual bool canCollide(OWCollisionData* other) = 0;
	virtual bool collides(OWCollisionData* other) = 0;
	virtual void collided(OWCollisionData* other) = 0;
};
