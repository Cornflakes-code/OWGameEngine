#pragma once
#include "../OWEngine/OWEngine.h"

#include <string>
#include <glm/glm.hpp>
#include <json/single_include/nlohmann/json.hpp>

#include "../Core/OWSceneComponent.h"

class OWENGINE_API LightSource : public OWSceneComponent
{
public:
	LightSource(OWActor* _owner, const glm::vec3& _position);
	~LightSource();
	void prepare();
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )
};


