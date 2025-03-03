#pragma once

#include <vector>

#include "../OWEngine/OWEngine.h"

class OWComponent;
class OWRay;
class OWRayActor;
namespace CollisionSystem
{
	void OWENGINE_API addCollider(OWCollider* coll, OWActor* a, int componentId);
	void OWENGINE_API addRay(OWCollider* coll, OWActor* a, int componentId);
	void OWENGINE_API deleteRay(OWRayActor* r);

	void OWENGINE_API collide();
	void OWENGINE_API refresh();
};
