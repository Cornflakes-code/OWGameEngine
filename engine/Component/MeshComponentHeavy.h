#pragma once
#include <vector>

#include "Helpers/MeshDataHeavy.h"
#include "OWSceneComponent.h"
#include <Helpers/Shader.h>

struct MeshDataHeavy;
class OLDActor;

struct OWENGINE_API MeshComponentHeavyData: public OLDSceneComponentData
{
	ShaderData shaderData;
	MeshDataHeavy meshData;
	unsigned int vertexMode;
	unsigned int vertexLocation;
};

class OWENGINE_API MeshComponentHeavy: public OLDSceneComponent
{
protected:
	MeshComponentHeavyData* data() override
	{
		return static_cast<MeshComponentHeavyData*>(OLDSceneComponent::data());
	}
public:
	MeshComponentHeavy(OLDActor* _owner, MeshComponentHeavyData* _data)
		: OLDSceneComponent(_owner, _data) {
	}
	const MeshComponentHeavyData* constData() const override
	{
		return static_cast<const MeshComponentHeavyData*>(OLDSceneComponent::constData());
	}
	void doInit() override;
private:
};
