#pragma once
#include <vector>

#include "Helpers/MeshDataLight.h"
#include "OWSceneComponent.h"
#include <Helpers/Shader.h>

struct MeshDataHeavy;
class OLDActor;

struct OWENGINE_API MeshComponentLightData: public OLDSceneComponentData
{
	ShaderData shaderData;
	MeshDataLight meshData;
};

class OWENGINE_API MeshComponentLight: public OLDSceneComponent
{
protected:
	MeshComponentLightData* data() override
	{
		return static_cast<MeshComponentLightData*>(OLDSceneComponent::data());
	}
public:
	MeshComponentLight(OLDActor* _owner, MeshComponentLightData* _data)
		: OLDSceneComponent(_owner, _data) {
	}
	const MeshComponentLightData* constData() const override
	{
		return static_cast<const MeshComponentLightData*>(OLDSceneComponent::constData());
	}
	void doInit() override;
private:
};
