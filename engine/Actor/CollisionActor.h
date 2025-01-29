#pragma once

#include <vector>

#include "../OWEngine/OWEngine.h"

#include "../Component/OWComponent.h"
#include "../Component/PhysicalComponent.h"
#include "../Component/OWSceneComponent.h"
#include "../Renderers/OWRenderable.h"

class OWRay;
class OWENGINE_API CollisionEngine : public OWComponent
{
public:
	CollisionEngine(OWActor* _owner)
		: OWComponent(_owner) {
	}
	void addStatic(OWSceneComponent* o);
	void addMoveableObject(OWSceneComponent* o);
	//int preTick() override;
	//int tick(float dt) override;
	//int postTick() override;
	std::vector< OWSceneComponent*> fireRay(OWRay* r);
};
