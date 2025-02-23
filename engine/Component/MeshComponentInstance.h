#pragma once
#include <vector>

#include "OWSceneComponent.h"
#include <Helpers/Shader.h>
#include <Renderers/InstanceRenderer.h>

class OLDActor;

struct OWENGINE_API MeshComponentInstanceData: public OLDSceneComponentData
{
	ShaderData shaderData;
	MeshDataInstance meshData;
};

class OWENGINE_API MeshComponentInstance: public OLDSceneComponent
{
protected:
	MeshComponentInstanceData* data() override
	{
		return static_cast<MeshComponentInstanceData*>(OLDSceneComponent::data());
	}
public:
	MeshComponentInstance(OLDActor* _owner, MeshComponentInstanceData* _data)
		: OLDSceneComponent(_owner, _data) 
	{}
	const MeshComponentInstanceData* constData() const override
	{
		return static_cast<const MeshComponentInstanceData*>(OLDSceneComponent::constData());
	}
	void doInit() override;
private:
};
