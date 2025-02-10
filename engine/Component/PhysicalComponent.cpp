#include "PhysicalComponent.h"


void OWIPhysical::setData(OWPhysicsData* newValue)
{ 
    mData = newValue;
    AABB bb = mData->boundingBox.findBoundsIfRotated(mData->physics.localMatrix);
    mData->boundingBox = bb;
}

void OWIPhysical::rotate(float radians, const glm::vec3& axis)
{
    mData->boundingBox = mData->boundingBox.findBoundsIfRotated(radians, axis);
    mData->physics.localMatrix = glm::rotate(mData->physics.localMatrix, radians, axis);
}
void OWIPhysical::scale(const glm::vec3& v)
{
    glm::vec3 center = mData->boundingBox.center();
    AABB bb = mData->boundingBox;
    bb.move(-center);
    bb = bb.scale(v);
    bb.move(center);
    mData->boundingBox = bb;
    mData->physics.localMatrix = glm::scale(mData->physics.localMatrix, v);
}
void OWIPhysical::translate(const glm::vec3& v)
{
    mData->boundingBox.move(v);
    mData->physics.localMatrix = glm::translate(mData->physics.localMatrix, v);
}

glm::vec3 OWIPhysical::scale() const
{
    // https://stackoverflow.com/questions/33538035/extracting-scale-matrix-from-modelview-matrix
    const glm::mat4& matrix = mData->physics.localMatrix;
    float xs =
        matrix[0][0] * matrix[0][1] * matrix[0][2] * matrix[0][3] < 0 ?
        -1 : 1;
    float ys =
        matrix[1][0] * matrix[1][1] * matrix[1][2] * matrix[1][3] < 0 ?
        -1 : 1;
    float zs =
        matrix[2][0] * matrix[2][1] * matrix[2][2] * matrix[2][3] < 0 ?
        -1 : 1;


    glm::vec3 new_scale;
    new_scale.x = xs * glm::sqrt(
        matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1]
        + matrix[0][2] * matrix[0][2]);
    new_scale.y = ys * glm::sqrt(
        matrix[1][0] * matrix[1][0] + matrix[1][1] * matrix[1][1]
        + matrix[1][2] * matrix[1][2]);
    new_scale.z = zs * glm::sqrt(
        matrix[2][0] * matrix[2][0] + matrix[2][1] * matrix[2][1]
        + matrix[2][2] * matrix[2][2]);
    return new_scale;
}
