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
//	glm::vec3 steerForce = glm::vec3(0); // These are all of the forces acting on the object's acceleration (thrust, gravity, drag, etc) 
};

// See https://gafferongames.com/post/fix_your_timestep/
// Always 
// 1. write to mCurrentState
// 2. read from mScratch AFTER interpolate called
// Also see run loop in Movie.cpp
class OWPhysics
{
	OWPhysicsData mPreviousData;
	OWPhysicsData mCurrentData;
	OWPhysicsData mScratchData;

	// 0(invisibility -> 1 (fully opaque)
	float mVisibility = 1.0f;
	float mMass = 1.0f;
	float mHardness = 0.5f;
public:
	OWPhysics(const OWPhysicsData& _data = OWPhysicsData())
		: mPreviousData(_data), mCurrentData(_data), mScratchData(_data)
	{
	}

	void tick(float dt)
	{
		// Only update mCurrentState
	}

	void copyCurrentToPrevious()
	{
		mPreviousData = mCurrentData;
	}

	void interpolate(float OW_UNUSED(totalTime), float alpha, float OW_UNUSED(fixedTimeStep))
	{
		float oneMinusAlpha = 1.0f - alpha;
		mScratchData.acceleration = mCurrentData.acceleration * alpha + mPreviousData.acceleration * oneMinusAlpha;
		mScratchData.velocity = mCurrentData.velocity * alpha + mPreviousData.velocity * oneMinusAlpha;
		mScratchData.rotationalVelocity = mCurrentData.rotationalVelocity * alpha + mPreviousData.rotationalVelocity * oneMinusAlpha;
	}

	glm::vec3 velocity() const {
		return mScratchData.velocity;
	}
	void visibility(float newValue)
	{
		if (newValue < 0.0f)
			newValue = 0.0f;
		else if (newValue > 1.0f)
			newValue = 1.0f;
		mVisibility = newValue;
	}
};
