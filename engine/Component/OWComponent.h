#pragma once

#include "../OWEngine/OWEngine.h"
#include "../Core/OWObject.h"

#include <string>

class OWActor;
class OWENGINE_API OWComponent: public OWObject
{
	OWActor* mActor = nullptr;
	bool mInitCalled = false;
protected:
	OWActor* actor() const { return mActor; }
public:
	OWComponent(OWActor* _actor = nullptr)
		:mActor(_actor)
	{
	}
	bool initCalled() const { return mInitCalled; }
	void actor(OWActor* _actor) { mActor = _actor; }
	void init()
	{
		if (!mInitCalled)
		{
			doInit();
			mInitCalled = true;
		}
	}
	virtual void doInit() = 0;
};

