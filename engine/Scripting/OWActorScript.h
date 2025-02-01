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

struct OWENGINE_API OWActorDataImp
{
	glm::vec3 position = glm::vec3(0);
	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 acceleration = glm::vec3(0);
	glm::vec3 orientation = glm::vec3(0);
	glm::vec3 rotationVelocity = glm::vec3(0);
	//glm::quat mOrientation = glm::quat();
	float mass = 1.0f;
	// 0(invisibility -> 1 (fully opaque)
	float visibility = 1.0f;
	glm::mat4 localMatrix = glm::mat4(1.0f);
};

struct OWENGINE_API OWActorData
{
	OWActorDataImp actorData;
};

class OWENGINE_API OWActorScript: public OWScript
{
	OWActorData* mData;
public: 		

	OWActorData* data()
	{
		return mData;
	}

	OWActorScript(OWActorData* _data)
		: mData(_data)
	{
	}
	const OWActorData* data() const
	{
		return mData;
	}
	friend class OWActor;
};
