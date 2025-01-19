#pragma once

#include "../OWEngine/OWEngine.h"
#include "../Core/OWObject.h"

class OWActor;
class OWENGINE_API OWComponent : public OWObject
{
protected:
	OWActor* mOwner;
public:
	OWComponent(OWActor* _owner)
		: mOwner(_owner) {}
	virtual int tick(float dt) = 0;
};
