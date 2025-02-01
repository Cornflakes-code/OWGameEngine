#include "MeshComponentVAO.h"


void MeshComponentVAO::doInit()
{
	MeshComponentVAOData* vao = data();
	if (vao->meshData.shader() == nullptr)
		vao->meshData.shader(new Shader(&vao->shaderData));
	vao->meshData.prepare();

	addRenderer(&vao->meshData);
}
