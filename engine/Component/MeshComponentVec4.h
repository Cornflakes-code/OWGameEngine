#pragma once
#include <vector>

#include "Helpers/MeshDataHeavy.h"
#include "OWSceneComponent.h"
#include <Helpers/Shader.h>

struct MeshDataHeavy;
class OWActor;

struct OWENGINE_API MeshComponentVec4Data: public OWSceneComponentData
{
	ShaderData* shaderData;
	std::vector<glm::vec4> meshData;
	unsigned int vertexMode;
	unsigned int vertexLocation;
};

class OWENGINE_API MeshComponentVec4: public OWSceneComponent
{
protected:
	MeshComponentVec4Data* data() override
	{
		return static_cast<MeshComponentVec4Data*>(data());
	}
public:
	MeshComponentVec4(OWActor* _owner, MeshComponentVec4Data* _data)
		: OWSceneComponent(_owner, _data) {
	}
	const MeshComponentVec4Data* constData() const override
	{
		return static_cast<const MeshComponentVec4Data*>(constData());
	}
	void init() override;
private:
};
