#pragma once
#include <string>
#include <glm/glm.hpp>
#include <json/single_include/nlohmann/json.hpp>

#include "../OWEngine/OWEngine.h"
#include "../Component/OWComponent.h"
#include "../Renderers/OWRenderable.h"
#include "../Helpers/Shader.h"

struct OWENGINE_API LightSourceData
{
	ShaderData shaderData;
	std::string name;
	glm::vec3 position;
};

class OWENGINE_API LightSource : public OLDComponent, public OLDIRenderable
{
	LightSourceData* mData;
public:
	LightSource(OLDActor* _owner, LightSourceData* _data);
	~LightSource();
	void doInit() override;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )
};


