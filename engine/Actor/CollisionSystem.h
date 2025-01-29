#pragma once

#include <vector>

#include "../OWEngine/OWEngine.h"

class OWSceneComponent;
class OWRay;
class OWIPhysical;
namespace CollisionSystem
{
	void OWENGINE_API build(std::vector<OWIPhysical*>& objects);
	void OWENGINE_API addRay(OWRay* r);
	void OWENGINE_API deleteRay(OWRay* r);

	void OWENGINE_API collide();
	void OWENGINE_API refresh();
};
