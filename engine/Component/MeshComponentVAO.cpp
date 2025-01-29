#include "MeshComponentVAO.h"


void MeshComponentVAO::init()
{
	MeshComponentVAOData* vao = data();
	if (vao->meshData.shader() == nullptr)
		vao->meshData.shader(new Shader(&vao->shaderData));
	vao->meshData.prepare();

	addRenderer(&vao->meshData);
}
