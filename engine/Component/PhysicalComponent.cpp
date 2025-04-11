#include "PhysicalComponent.h"


void OWPhysics::tick(float timeStep)
{
	mCurrentData.pd.velocity += mCurrentData.pd.acceleration * timeStep;
	mCurrentData.pd.rotationalVelocity += mCurrentData.pd.rotationalAccelearation * timeStep;
	glm::vec3 pos = mCurrentData.t->localPosition();
	pos += mCurrentData.pd.velocity * timeStep;
	mCurrentData.t->localPosition(pos);
}

void OWPhysics::interpolate(float OW_UNUSED(totalTime), float alpha, float timeStep)
{
	float oneMinusAlpha = 1.0f - alpha;
	mScratchData.pd.acceleration = mCurrentData.pd.acceleration * alpha + mPreviousData.pd.acceleration * oneMinusAlpha;
	mScratchData.pd.velocity = mCurrentData.pd.velocity * alpha + mPreviousData.pd.velocity * oneMinusAlpha;
	mScratchData.pd.rotationalVelocity = mCurrentData.pd.rotationalVelocity * alpha + mPreviousData.pd.rotationalVelocity * oneMinusAlpha;
	glm::vec3 pos = mPreviousData.t->localPosition();
	pos += mScratchData.pd.velocity * timeStep;
	mScratchData.t->localPosition(pos);
}