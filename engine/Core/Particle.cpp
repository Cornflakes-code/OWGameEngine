#include "Particle.h"

#include <glm/gtc/epsilon.hpp>

static constexpr float gEpsilon = 0.0001f;
glm::vec3 gZero = glm::vec3(0.0f);

bool Physical::move(float dt)
{
    bool retval = glm::epsilonEqual(dt, 0.0f, 0.0000000f) &&
                    glm::all(glm::epsilonEqual(mCurrent.mVelocity, gZero, gEpsilon));
    mPrevious = mCurrent;
    glm::vec3 moveStep = mCurrent.mVelocity * dt;
    mCurrent.mPosition += moveStep;
    mBoundingSphere.move(moveStep);
    mHitSphere.move(moveStep);
    mBoundingBox.move(moveStep);
    mHitBox.move(moveStep);

    mCurrent.mVelocity += mCurrent.mAcceleration * dt;
    return retval;
}
