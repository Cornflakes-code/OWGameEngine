#include "PhysicalComponent.h"


void OWPhysicsDataImp::translate(const glm::vec3& newValue)
{
    mTranslate += newValue;
}

void OWPhysicsDataImp::rotate(float radians, const glm::vec3& axis)
{
    mRotate += axis * radians;
}

void OWPhysicsDataImp::scale(const glm::vec3& newValue)
{
    mScale *= newValue;
}

void OLDIPhysical::setData(OWPhysicsData* newValue)
{ 
    mData = newValue;
}

void OLDIPhysical::physicalDoInit()
{
    // bb needs to catch up on its settings
    float len = glm::length(data()->physics.mRotate);
    if (!OWUtils::isZero(len))
    {
        glm::vec3 norm = glm::normalize(data()->physics.mRotate);
        mData->boundingBox = mData->boundingBox.findBoundsIfRotated(len, norm);
    }

    glm::vec3 center = mData->boundingBox.center();
    AABB bb = mData->boundingBox;
    mData->boundingBox.move(-center);
    mData->boundingBox.scale(data()->physics.mScale);
    mData->boundingBox.move(center);

    mData->boundingBox.move(data()->physics.mTranslate);
}

void OLDIPhysical::rotate(float radians, const glm::vec3& axis)
{
    if (!OWUtils::isZero(radians))
    {
        mData->physics.rotate(radians, axis);
    }
}

void OLDIPhysical::scale(const glm::vec3& v)
{
    glm::vec3 center = mData->boundingBox.center();
    mData->boundingBox.move(-center);
    mData->boundingBox.scale(v);
    mData->boundingBox.move(center);
    mData->physics.scale(v);
}
void OLDIPhysical::translate(const glm::vec3& v)
{
    mData->boundingBox.move(v);
    mData->physics.translate(v);
}

glm::vec3 OLDIPhysical::scale() const
{
    return mData->physics.mScale;
}
