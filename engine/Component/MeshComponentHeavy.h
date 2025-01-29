#pragma once
#include <vector>

#include "Helpers/MeshDataHeavy.h"
#include "OWSceneComponent.h"
#include <Helpers/Shader.h>

struct MeshDataHeavy;
class OWActor;

struct OWENGINE_API MeshComponentHeavyData: public OWSceneComponentData
{
	ShaderData shaderData;
	MeshDataHeavy meshData;
	unsigned int vertexMode;
	unsigned int vertexLocation;
};

class OWENGINE_API MeshComponentHeavy: public OWSceneComponent
{
protected:
	MeshComponentHeavyData* data() override
	{
		return static_cast<MeshComponentHeavyData*>(OWSceneComponent::data());
	}
public:
	MeshComponentHeavy(OWActor* _owner, MeshComponentHeavyData* _data)
		: OWSceneComponent(_owner, _data) {
	}
	const MeshComponentHeavyData* constData() const override
	{
		return static_cast<const MeshComponentHeavyData*>(OWSceneComponent::constData());
	}
	void init() override;
private:
};
