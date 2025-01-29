#pragma once
#include <vector>

#include "OWSceneComponent.h"

#include <Renderers/VAOBuffer.h>
#include <Helpers/Shader.h>

class OWActor;

struct OWENGINE_API MeshComponentVAOData: public OWSceneComponentData
{
	ShaderData shaderData;
	VAOBuffer meshData;
	unsigned int vertexMode;
	unsigned int vertexLocation;
};

class OWENGINE_API MeshComponentVAO: public OWSceneComponent
{
protected:
	MeshComponentVAOData* data() override
	{
		return static_cast<MeshComponentVAOData*>(OWSceneComponent::data());
	}
public:
	MeshComponentVAO(OWActor* _owner, MeshComponentVAOData* _data)
		: OWSceneComponent(_owner, _data) {
	}
	const MeshComponentVAOData* constData() const override
	{
		return static_cast<const MeshComponentVAOData*>(OWSceneComponent::constData());
	}
	void init() override;
private:
};
