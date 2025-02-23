#pragma once

#include "../OWEngine/OWEngine.h"
#include "../Core/OWObject.h"

#include <string>

class OLDActor;
class OWENGINE_API OLDComponent: public OLDObject
{
	OLDActor* mActor = nullptr;
	bool mInitCalled = false;
protected:
	OLDActor* actor() const { return mActor; }
public:
	OLDComponent(OLDActor* _actor = nullptr)
		:mActor(_actor)
	{
	}
	bool initCalled() const { return mInitCalled; }
	void actor(OLDActor* _actor) { mActor = _actor; }
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

