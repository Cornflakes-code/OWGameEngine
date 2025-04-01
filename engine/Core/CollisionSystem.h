#pragma once

#include <vector>

#include "../OWEngine/OWEngine.h"

class OWCollider;
class OWRay;
class OWActor;
namespace CollisionSystem
{
	void OWENGINE_API addCollider(OWCollider* coll, OWActor* a, int componentId);
	void OWENGINE_API addRay(OWCollider* coll, OWActor* a, int componentId);
	void OWENGINE_API deleteRay(OWActor* r);

	void OWENGINE_API collide();
	void OWENGINE_API preTick();
	void OWENGINE_API postTick();
	void OWENGINE_API tick();
};
