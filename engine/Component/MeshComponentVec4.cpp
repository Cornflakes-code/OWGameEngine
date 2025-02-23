#include "MeshComponentVec4.h"

#include <Renderers/LightRenderer.h>

void MeshComponentVec4::doInit()
{
	MeshComponentVec4Data* d = data();
	Shader* shader = new Shader(&d->shaderData);
	LightRenderer* r = new LightRenderer(shader);
	r->setup(d->meshData, d->vertexMode, d->vertexLocation);
	d->boundingBox = convertToV3(d->meshData);
	addRenderer(r);
	OLDSceneComponent::doInit();
}
