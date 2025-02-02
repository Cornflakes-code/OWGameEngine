#include "PhysicalComponent.h"

	glm::vec3 OWPhysicalMetaData::gravity = glm::vec3(0.0f, 9.8f, 0.0f);
	glm::vec3 OWPhysicalMetaData::maxVelocity = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 OWPhysicalMetaData::maxAcceleration = glm::vec3(std::numeric_limits<float>::max());
	float OWPhysicalMetaData::maxMass = std::numeric_limits<float>::max();
	float OWPhysicalMetaData::hardness = 0.5f;


	glm::vec3 OWPhysicsDataImp::scale() const
	{
        // https://stackoverflow.com/questions/33538035/extracting-scale-matrix-from-modelview-matrix
        const glm::mat4& matrix = localMatrix;
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
