#pragma once
#include "../OWEngine/OWEngine.h"

#include <string>

#include <json/single_include/nlohmann/json.hpp>

#include <glm/glm.hpp>

class LightRenderer;

class OWENGINE_API LightSource //: public ResourceSource
{
public:
	LightSource();
	~LightSource();
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )

	LightRenderer* mLightSource = NULL;
};


