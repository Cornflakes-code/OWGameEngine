#include "OWActorScript.h"

#include "../Actor/OWActor.h"

void OWScriptComponent::tick(float dt)
{
	// Interate through the script, getting ndx
	RequiredComponents required;
	int ndx = 42;
	mActor->getScriptingComponents(ndx, required);
}
