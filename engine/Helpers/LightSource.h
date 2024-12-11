#pragma once
#include "../OWEngine/OWEngine.h"

#include <string>
#include <glm/glm.hpp>
#include <json/single_include/nlohmann/json.hpp>

#include "../Core/Actor.h"

class LightRenderer;
class Physical;

class OWENGINE_API LightSource : public Actor
{
public:
	LightSource(Physical* ph, Actor* _owner);
	~LightSource();
	void prepare();
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )
};


