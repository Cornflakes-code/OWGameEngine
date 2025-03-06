#pragma once
#include <string>

#include "../OWEngine/OWEngine.h"
#include "../OWEngine/OWGameIFace.h"

class OWENGINE_API OWScriptComponent
{
public:
};

class OWENGINE_API OLDScript: public OLDGameIFace
{
public:
	OLDScript() {}
	virtual ~OLDScript() {}
	virtual void tick(float deltaSecods) override;
	virtual void begin() override;
	virtual void end() override;
	virtual void destroy() override;
protected:
	virtual void doInit() override;
};
