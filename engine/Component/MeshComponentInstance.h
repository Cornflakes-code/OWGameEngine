#pragma once
#include <vector>

#include "OWSceneComponent.h"
#include <Helpers/Shader.h>
#include <Renderers/InstanceRenderer.h>

class OWActor;

struct OWENGINE_API MeshComponentInstanceData: public OWSceneComponentData
{
	ShaderData shaderData;
	MeshDataInstance meshData;
};

class OWENGINE_API MeshComponentInstance: public OWSceneComponent
{
protected:
	MeshComponentInstanceData* data() override
	{
		return static_cast<MeshComponentInstanceData*>(OWSceneComponent::data());
	}
public:
	MeshComponentInstance(OWActor* _owner, MeshComponentInstanceData* _data)
		: OWSceneComponent(_owner, _data) 
	{}
	const MeshComponentInstanceData* constData() const override
	{
		return static_cast<const MeshComponentInstanceData*>(OWSceneComponent::constData());
	}
	void doInit() override;
private:
};
