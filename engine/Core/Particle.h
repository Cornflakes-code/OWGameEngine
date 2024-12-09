#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>

#include "Actor.h"
#include "BoundingBox.h"
#include "Ray.h"
#include "BoundingSphere.h"
#include "CommonUtils.h"
#include "Camera.h"


class PhysicalTypeMetaData
{
public:
    enum ThreeWay { Yes, No, Maybe };
    glm::vec3 mGravity = glm::vec3(0);
    glm::vec3 mMaxVelosity = glm::vec3(0);
    glm::vec3 mAcceleration = glm::vec3(0);
    float mHardness = 0.0f;
    ThreeWay mVisible = ThreeWay::Maybe;
};

class ParticleData
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

class Physical
{
public:
    double          n;              // User's unique identifier
    unsigned int code;           // Used during octree generation
    const AABB& bounds() const { return mBoundingBox; }
private:
    ParticleData* mCurrent = nullptr;
    ParticleData* mPrevious = nullptr;
    PhysicalTypeMetaData* mClassSpecs = nullptr;
    BoundingSphere mBoundingSphere = BoundingSphere(glm::vec3(0), 1.0f);
    BoundingSphere mHitSphere = BoundingSphere(glm::vec3(0), 1.0f);
    glm::vec3 mHitSphereOffset = glm::vec3(0);
    AABB mBoundingBox;
    AABB mHitBox;
    glm::vec3 mSteerForce = glm::vec3(0);       //These are all of the forces acting on the object accelleration (thrust, gravity, drag, etc) 
    float mScale = 1.0f;         //this is the scale factor for the object
    };
