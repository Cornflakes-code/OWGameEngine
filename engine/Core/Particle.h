#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>

#include "BoundingBox.h"
#include "Ray.h"
#include "BoundingSphere.h"
#include "CommonUtils.h"
#include "Camera.h"


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

class OWENGINE_API Collidable
{
protected:
    AABB mBoundingBox = AABB(glm::vec3(-1), glm::vec3(-1));
public:
    const AABB& bounds() const { return mBoundingBox; }
    void bounds(const AABB& bb) { mBoundingBox = bb; }
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

class OWENGINE_API Physical: public Collidable
{
private:
    ParticleData mCurrent;
    ParticleData mPrevious;
    PhysicalTypeMetaData* mClassSpecs = nullptr;
    BoundingSphere mBoundingSphere = BoundingSphere(glm::vec3(0), 1.0f);
    BoundingSphere mHitSphere = BoundingSphere(glm::vec3(0), 1.0f);
    glm::vec3 mHitSphereOffset = glm::vec3(0);
    AABB mHitBox;
    glm::vec3 mSteerForce = glm::vec3(0);       //These are all of the forces acting on the object accelleration (thrust, gravity, drag, etc) 
    float mScale = 1.0f;         //this is the scale factor for the object
public:
    Physical(const glm::vec3& _position) { position(_position); }
    Physical() {}
    const glm::vec3& position() const { return mCurrent.mPosition; }
    void position(const glm::vec3& value) { mCurrent.mPosition = value; }
    bool move(float dt); // return true if moved
    void velocity(const glm::vec3& direction, float speed)
    {
        velocity(glm::normalize(direction) * speed);
    }
    void velocity(const glm::vec3& value)
    {
        // check does not exceed max velocity
        mCurrent.mVelocity = value;
    }
};
