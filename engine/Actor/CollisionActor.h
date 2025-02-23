#pragma once

#include <vector>

#include "../OWEngine/OWEngine.h"

#include "../Component/OWComponent.h"
#include "../Component/PhysicalComponent.h"
#include "../Component/OWSceneComponent.h"
#include "../Renderers/OWRenderable.h"

class OWRay;
class OWENGINE_API CollisionEngine : public OLDComponent
{
public:
	CollisionEngine(OLDActor* _owner)
		: OLDComponent(_owner) {
	}
	void addStatic(OLDSceneComponent* o);
	void addMoveableObject(OLDSceneComponent* o);
	//int preTick() override;
	//int tick(float dt) override;
	//int postTick() override;
	std::vector< OLDSceneComponent*> fireRay(OWRay* r);
};
