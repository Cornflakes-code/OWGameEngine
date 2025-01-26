#pragma once
#include <string>
#include <limits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../OWEngine/OWEngine.h"
#include "OWScript.h"

const struct OWENGINE_API OWActorMetaData
{
	glm::vec3 Gravity = glm::vec3(0.0f, 9.8f, 0.0f);
	glm::vec3 MaxVelocity = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 MaxAcceleration = glm::vec3(std::numeric_limits<float>::max());
	float MaxMass = std::numeric_limits<float>::max();
	float Hardness = 0.5f;
};

struct OWENGINE_API OWActorData
{
	glm::vec3 Velocity = glm::vec3(0);
	glm::vec3 Acceleration = glm::vec3(0);
	glm::vec3 Orientation = glm::vec3(0);
	glm::vec3 RotationVelocity = glm::vec3(0);
	//glm::quat mOrientation = glm::quat();
	float mMass = 1.0f;
	// 0(invisible -> 1 (fully opaque)
	float Visible = 1.0f;
	glm::mat4 LocalMatrix = glm::mat4(1.0f);
};

class OWENGINE_API OWActorScript: public OWScript
{
	OWActorData* mPhysicalData;
public: 		

	OWActorData* data()
	{
		return mPhysicalData;
	}

	OWActorScript(OWActorData* _data)
	{
		mPhysicalData = _data = nullptr ? new OWActorData() : _data;
	}
	const OWActorData* data() const
	{
		return mPhysicalData;
	}
	friend class OWActor;
};
