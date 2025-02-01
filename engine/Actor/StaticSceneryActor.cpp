#include "StaticSceneryActor.h"

StaticSceneryActor::StaticSceneryActor(Scene* _scene, StaticSceneryScript* _script)
	: OWActor(_scene, _script)
{
	name("Scenery");
}

void StaticSceneryActor::doInit()
{
	OWActor::doInit();
}
