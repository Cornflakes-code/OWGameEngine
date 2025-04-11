#include "OWActorScript.h"

#include "../Actor/OWActor.h"

OWScriptComponent::OWScriptComponent(const std::string& fileName)
{
}

OWScriptComponent::OWScriptComponent()
{
}

void OWScriptCommmand::execute()
{
}
void OWScriptComponent::tick(float dt)
{
	// Interate through the script, getting ndx
	OWScriptComponent::RequiredComponents c;
	int ndx = 42;
	// jfw todo
	mActor->getScriptingComponents(ndx, c);

	for (auto& command : mCommands)
	{

	}
}
