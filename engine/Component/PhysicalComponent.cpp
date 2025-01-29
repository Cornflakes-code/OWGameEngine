#include "PhysicalComponent.h"

	glm::vec3 OWPhysicalMetaData::gravity = glm::vec3(0.0f, 9.8f, 0.0f);
	glm::vec3 OWPhysicalMetaData::maxVelocity = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 OWPhysicalMetaData::maxAcceleration = glm::vec3(std::numeric_limits<float>::max());
	float OWPhysicalMetaData::maxMass = std::numeric_limits<float>::max();
	float OWPhysicalMetaData::hardness = 0.5f;
