#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "../OWEngine/OWEngine.h"
#include "OWComponent.h"
#include "../Geometry/BoundingBox.h"

namespace OWMoveMetaData
{
	glm::vec3 gravity = glm::vec3(0.0f, 9.8f, 0.0f);
	glm::vec3 maxVelocity = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxAcceleration = glm::vec3(std::numeric_limits<float>::max());
	float maxMass = std::numeric_limits<float>::max();
	float hardness = 0.5f;
	enum ChangeType { increment, absolute };
};

class OWIMovable;
struct OWENGINE_API OWCollisionData
{
	AABB boundingBox = AABB(glm::vec3(-1), glm::vec3(-1));
	OWIMovable* component = nullptr;
	bool canMove = true;
	bool canCollide = true;
};

struct OWENGINE_API OWMoveData: public OWCollisionData
{
	glm::vec3 position = glm::vec3(0);
	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 acceleration = glm::vec3(0);
	glm::vec3 orientation = glm::vec3(0);
	glm::vec3 rotationVelocity = glm::vec3(0);
	//glm::quat mOrientation = glm::quat();
	float mass = 1.0f;
	// 0(invisible -> 1 (fully opaque)
	float visible = 1.0f;
	glm::mat4 localMatrix = glm::mat4(1.0f);
	glm::vec3 steerForce = glm::vec3(0);       //These are all of the forces acting on the object accelleration (thrust, gravity, drag, etc) 
};

class OWENGINE_API OWIMovable
{
	OWMoveData* mData = nullptr;
protected:
public:
	virtual OWMoveData* data() { return mData; }
	virtual const OWMoveData* constData() const { return mData; }

	// OWCollisionData has a smaller footprint than OWMoveData.
	// Use it for the collision engine
	OWCollisionData* collisionData() { return mData; }
	void setData(OWMoveData* newValue) { mData = newValue; }
	virtual bool canCollide() = 0;
	virtual bool canCollide(OWCollisionData* other) = 0;
	virtual bool collides(OWCollisionData* other) = 0;
	virtual void collided(OWCollisionData* other) = 0;
};
