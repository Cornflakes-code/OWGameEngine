#pragma once

#include <vector>

#include "../OWEngine/OWEngine.h"

class OWSceneComponent;
class OWRay;
struct OWCollisionData;
namespace CollisionSystem
{
	void OWENGINE_API tick(float timeStep);
	void OWENGINE_API build(std::vector<OWCollisionData*>& objects);
	void OWENGINE_API addRay(OWRay* r);
	void OWENGINE_API deleteRay(OWRay* r);

	void OWENGINE_API collide();
	void OWENGINE_API refresh();
};
