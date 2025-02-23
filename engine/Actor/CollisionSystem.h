#pragma once

#include <vector>

#include "../OWEngine/OWEngine.h"

class OLDSceneComponent;
class OWRay;
class OLDIPhysical;
namespace CollisionSystem
{
	void OWENGINE_API build(std::vector<OLDIPhysical*>& objects);
	void OWENGINE_API addRay(OWRay* r);
	void OWENGINE_API deleteRay(OWRay* r);

	void OWENGINE_API collide();
	void OWENGINE_API refresh();
};
