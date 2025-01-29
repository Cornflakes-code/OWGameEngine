#pragma once
#include <string>

#include "../OWEngine/OWEngine.h"
#include "../OWEngine/OWGameIFace.h"
#include "../Component/OWComponent.h"

class OWENGINE_API OWScript: public OWGameIFace
{
public:
	OWScript() {}
	virtual ~OWScript() {}
	virtual void tick(float deltaSecods) override;
	virtual void begin() override;
	virtual void end() override;
	virtual void destroy() override;
protected:
	virtual void doInit() override;
};
