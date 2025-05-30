#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../OWEngine/OWEngine.h"
#include "Component.h"
#include "../Geometry/BoundingBox.h"
#include "../Helpers/Transform.h"

struct OWENGINE_API OWPhysicsData
{
	glm::vec3 rotationalAcceleration = glm::vec3(0); // radians per second per second
	glm::vec3 rotationalVelocity = glm::vec3(0); // radians per second
	glm::vec3 acceleration = glm::vec3(0);
	glm::vec3 velocity = glm::vec3(0);
	//	glm::vec3 steerForce = glm::vec3(0); // These are all of the forces acting on the object's acceleration (thrust, gravity, drag, etc) 
};

// See https://gafferongames.com/post/fix_your_timestep/
// Always 
// 1. write to mCurrentState
// 2. read from mScratch AFTER interpolate called
// Also see run loop in Movie.cpp
class OWENGINE_API OWPhysics
{
	struct OWPhysicsElement
	{
		OWTransform* t;
		OWPhysicsData pd;
	};
	OWPhysicsElement mPreviousData;
	OWPhysicsElement mCurrentData;
	OWPhysicsElement mScratchData;

	// 0(invisibility -> 1 (fully opaque)
	float mVisibility = 1.0f;
	float mMass = 1.0f;
	float mHardness = 0.5f;
public:
	OWPhysics(OWTransform* _tr, const OWPhysicsData& _data = OWPhysicsData())
		: mPreviousData({ _tr, _data }), mCurrentData({ _tr, _data }), mScratchData({ _tr, _data })
	{
		if (_tr == nullptr)
		{
			mPreviousData.t = mCurrentData.t = mScratchData.t = new OWTransform();
		}
	}
	const OWTransform* transform() const { return mCurrentData.t; }
	OWTransform* transform() { return mCurrentData.t; }

	const OWTransform* renderTransform() const { return mScratchData.t; }

	void tick(float timeStep);
	void copyCurrentToPrevious()
	{
		mPreviousData.pd = mCurrentData.pd;
	}

	void interpolate(float totalTime, float alpha, float timeStep);
	void visibility(float newValue)
	{
		if (newValue < 0.0f)
			newValue = 0.0f;
		else if (newValue > 1.0f)
			newValue = 1.0f;
		mVisibility = newValue;
	}
private:
	static void calcChanges(OWPhysicsElement& target, const OWPhysicsElement& alphaSource,
		const OWPhysicsElement& otherSource, float alpha, float timeStep);

};
