#pragma once
#include <string>

#include "../OWEngine/OWEngine.h"
#include "../OWEngine/OWGameIFace.h"
#include "../Component/OWComponent.h"

class OWENGINE_API OWScript: public OWComponent, public OWGameIFace
{
public:
	OWScript() {}
	virtual ~OWScript() {}
};
