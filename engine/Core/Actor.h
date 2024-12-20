#pragma once

#include <vector>
#include <string>

#include "../OWEngine/OWEngine.h"
//#include "BoundingBox.h"
#include "OWActor.h"

class Scene;
class OWENGINE_API ActorX: public OWActor
{
private:
public:
	/// 0 = The object did not move.
	/// 1 = the object was moved.
	/// 2 = the object just died.
	int tick(float dt, TickType tt = InitialTick) override
	{ 
		return OWActor::tick(dt, tt);
	}
protected:
	ActorX(Scene* _scene)
		:OWActor(_scene, glm::vec3(0)) {}
};
