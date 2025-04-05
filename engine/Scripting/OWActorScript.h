#pragma once
#include <string>
#include <limits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../OWEngine/OWEngine.h"


class OWActor;
class OWPhysics;
class OWMeshComponentBase;
class OWTransform;
class OWSoundComponent;

class OWENGINE_API OWScriptComponent
{
	OWActor* mActor = nullptr;
public:
	OWScriptComponent(OWActor* a)
		: mActor(a) {
	}
	struct OWENGINE_API RequiredComponents
	{
		OWPhysics* phys;
		OWMeshComponentBase* mesh = nullptr;;
		OWTransform* trans = nullptr;
		OWSoundComponent* sound = nullptr;
	};

	void tick(float dt);
};
