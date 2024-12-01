#pragma once
#include "../OWEngine/OWEngine.h"
#include "../Core/Actor.h"

#include <string>

#include <json/single_include/nlohmann/json.hpp>

#include <glm/glm.hpp>

class LightRenderer;

class OWENGINE_API LightSource : public Actor
{
public:
	LightSource(const glm::vec3& _position);
	~LightSource();
	void prepare();
private:
	glm::vec3 mPosition;
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )
};


