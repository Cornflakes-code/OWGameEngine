#pragma once
#include <vector>

#include "Helpers/MeshDataHeavy.h"
#include "OWSceneComponent.h"
#include <Helpers/Shader.h>

struct MeshDataHeavy;
class OLDActor;

struct OWENGINE_API MeshComponentVec4Data: public OLDSceneComponentData
{
	ShaderData shaderData;
	std::vector<glm::vec4> meshData;
	unsigned int vertexMode;
	unsigned int vertexLocation;
};

class OWENGINE_API MeshComponentVec4: public OLDSceneComponent
{
protected:
	MeshComponentVec4Data* data() override
	{
		return static_cast<MeshComponentVec4Data*>(OLDSceneComponent::data());
	}
public:
	MeshComponentVec4(OLDActor* _owner, MeshComponentVec4Data* _data)
		: OLDSceneComponent(_owner, _data) {
	}
	const MeshComponentVec4Data* constData() const override
	{
		return static_cast<const MeshComponentVec4Data*>(OLDSceneComponent::constData());
	}
	void doInit() override;
private:
};
