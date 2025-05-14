#include "PhysicalComponent.h"

#define _USE_MATH_DEFINES
#include <math.h>

struct OWENGINE_API OWPhysicalDataMaximums
{
	glm::vec3 gravity = glm::vec3(0.0f, 9.8f, 0.0f);
	glm::vec3 velocity = glm::vec3(1000.0f);
	glm::vec3 acceleration = glm::vec3(100.0f);
	glm::vec3 rotationalAcceleration = glm::vec3(1.0f);
	glm::vec3 rotationalVelocity = glm::vec3(10.0f);
	float mass = 1.0f;
	float hardness = 1.0f;
};

OWPhysicalDataMaximums gMaximums;
float TWO_PI = M_PI * 2.0f;
glm::vec3 gMaxRotAngle = glm::vec3(TWO_PI, TWO_PI, TWO_PI);

// for shaders
//glm::vec3 quat_transform(glm::vec4 q, glm::vec3 v)
//{ return v + 2. * glm::cross(q.xyz, glm::cross(q.xyz, v) + q.w * v); }
 


// Function to convert rotational velocity to a quaternion
glm::quat rotationalVelocityToQuaternion(glm::vec3 velocity, float deltaTime) 
{
	// https://www.google.com/search?client=firefox-b-d&q=opengl+converting+rotation+velocity+to+quaterion
	// 
	if (OWUtils::isZero(velocity))
	{
		return glm::quat(0, 0, 0, 0);
	}
	// 1. Normalize the velocity vector (to get the rotation axis)
	glm::vec3 axis = glm::normalize(velocity);
	
	// 2. Calculate the rotation angle (in radians)
	float angle = glm::length(velocity) * deltaTime;
	
	// 3. Calculate the quaternion components
	float halfAngle = angle / 2.0f;
	float w = glm::cos(halfAngle);
	glm::vec3 v = axis * glm::sin(halfAngle);

	return glm::quat(w, v.x, v.y, v.z);
}

void OWPhysics::calcChanges(OWPhysicsElement& target, const OWPhysicsElement& alphaSource,
				const OWPhysicsElement& otherSource, float alpha, float timeStep)
{
	const float oneMinusAlpha = 1.0f - alpha;
	target.pd.acceleration = alphaSource.pd.acceleration * alpha + otherSource.pd.acceleration * oneMinusAlpha;
	target.pd.velocity = alphaSource.pd.velocity * alpha + otherSource.pd.velocity * oneMinusAlpha
		+ target.pd.acceleration * timeStep;

	glm::vec3 pos = alphaSource.t->localPosition() * alpha + otherSource.t->localPosition() * oneMinusAlpha;
	pos += target.pd.velocity * timeStep;
	target.t->localPosition(pos);

	target.pd.rotationalAcceleration = alphaSource.pd.rotationalAcceleration * alpha 
		+ otherSource.pd.rotationalAcceleration * oneMinusAlpha;
	target.pd.rotationalVelocity = alphaSource.pd.rotationalVelocity * alpha
		+ otherSource.pd.rotationalVelocity * oneMinusAlpha;
	target.pd.rotationalVelocity += target.pd.rotationalAcceleration * timeStep;
	target.pd.rotationalVelocity = glm::clamp(target.pd.rotationalVelocity, -gMaximums.rotationalVelocity, gMaximums.rotationalVelocity);
	glm::vec3 rotVel = glm::clamp(target.pd.rotationalVelocity, -gMaxRotAngle, gMaxRotAngle);

	glm::vec3 rot = alphaSource.t->rotation() * alpha + otherSource.t->rotation() * oneMinusAlpha;
	rot += rotVel * timeStep;
	target.t->rotation(rot);
}

void OWPhysics::tick(float timeStep)
{
	calcChanges(mCurrentData, mCurrentData, mCurrentData, 1.0f, timeStep);
}

void OWPhysics::interpolate(float OW_UNUSED(totalTime), float alpha, float timeStep)
{
	calcChanges(mScratchData, mCurrentData, mPreviousData, alpha, timeStep);
}