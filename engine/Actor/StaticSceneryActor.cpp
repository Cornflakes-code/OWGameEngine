#include "StaticSceneryActor.h"

StaticSceneryActor::StaticSceneryActor(Scene* _scene, StaticSceneryScript* _script)
	: OLDActor(_scene, _script)
{
	name("Scenery");
}

void StaticSceneryActor::doInit()
{
	OLDActor::doInit();
}
