#pragma once

#include "../Core/OWObject.h"
#include <vector>

#include "../Actor/OWActor.h"
#include "../OWEngine/OWEngine.h"
#include "../Scripting/OWActorScript.h"
#include "../Component/OWComponent.h"
#include "../Component/PhysicalComponent.h"
#include "../Component/OWSceneComponent.h"
#include "../Renderers/OWRenderable.h"

struct OWENGINE_API StaticSceneryDataImp
{
};

struct OWENGINE_API StaticSceneryData: public OWActorData
{
	StaticSceneryDataImp sceneryData;
};

class OWENGINE_API StaticSceneryScript: public OWActorScript
{
public:
	StaticSceneryScript(StaticSceneryData* _data)
		: OWActorScript(_data) {
	}
};

class OWENGINE_API StaticSceneryActor: public OWActor
{
protected:
	StaticSceneryData* data()
	{
		return static_cast<StaticSceneryData*>(script()->data());
	}
public:
	StaticSceneryActor(Scene* _scene, StaticSceneryScript* _script);
	const StaticSceneryData* constData() const
	{
		return static_cast<const StaticSceneryData*>(script()->data());
	}
	void doInit() override;
};
