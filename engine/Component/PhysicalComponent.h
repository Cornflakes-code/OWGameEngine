#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "../OWEngine/OWEngine.h"
#include "Component.h"
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
//	glm::vec3 steerForce = glm::vec3(0); // These are all of the forces acting on the object accelleration (thrust, gravity, drag, etc) 
};

class OWPhysics
{
	OWPhysicsData mData;
public:
	OWPhysics(const OWPhysicsData& _data = OWPhysicsData())
		: mData(_data)
	{
	}
	void visibility(float newValue)
	{
		if (newValue < 0.0f)
			newValue = 0.0f;
		else if (newValue > 1.0f)
			newValue = 1.0f;
		mData.visibility = newValue;
	}
};
