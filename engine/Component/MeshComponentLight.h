#pragma once
#include <vector>

#include "Helpers/MeshDataLight.h"
#include "OWSceneComponent.h"
#include <Helpers/Shader.h>

struct MeshDataHeavy;
class OWActor;

struct OWENGINE_API MeshComponentLightData: public OWSceneComponentData
{
	ShaderData shaderData;
	MeshDataLight meshData;
};

class OWENGINE_API MeshComponentLight: public OWSceneComponent
{
protected:
	MeshComponentLightData* data() override
	{
		return static_cast<MeshComponentLightData*>(OWSceneComponent::data());
	}
public:
	MeshComponentLight(OWActor* _owner, MeshComponentLightData* _data)
		: OWSceneComponent(_owner, _data) {
	}
	const MeshComponentLightData* constData() const override
	{
		return static_cast<const MeshComponentLightData*>(OWSceneComponent::constData());
	}
	void init() override;
private:
};
