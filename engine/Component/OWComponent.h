#pragma once

#include "../OWEngine/OWEngine.h"
#include "../Core/OWObject.h"

#include <string>

class OWActor;
class OWENGINE_API OWComponent: public OWObject
{
	OWActor* mActor = nullptr;
protected:
	OWActor* actor() const { return mActor; }
public:
	OWComponent(OWActor* _actor = nullptr)
		:mActor(_actor)
	{
	}
	void actor(OWActor* _actor) { mActor = _actor; }
	virtual void init() = 0;
};

