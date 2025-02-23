#pragma once
#include <vector>

#include "OWSceneComponent.h"

#include <Renderers/VAOBuffer.h>
#include <Helpers/Shader.h>

class OLDActor;

struct OWENGINE_API MeshComponentVAOData: public OLDSceneComponentData
{
	ShaderData shaderData;
	VAOBuffer meshData;
	unsigned int vertexMode;
	unsigned int vertexLocation;
};

class OWENGINE_API MeshComponentVAO: public OLDSceneComponent
{
protected:
	MeshComponentVAOData* data() override
	{
		return static_cast<MeshComponentVAOData*>(OLDSceneComponent::data());
	}
public:
	MeshComponentVAO(OLDActor* _owner, MeshComponentVAOData* _data)
		: OLDSceneComponent(_owner, _data) {
	}
	const MeshComponentVAOData* constData() const override
	{
		return static_cast<const MeshComponentVAOData*>(OLDSceneComponent::constData());
	}
	void doInit() override;
private:
};
