#include "MeshComponentHeavy.h"

#include <Renderers/HeavyRenderer.h>

void MeshComponentHeavy::doInit()
{
	MeshComponentHeavyData* d = data();
	Shader* shader = new Shader(&d->shaderData);
	HeavyRenderer* r = new HeavyRenderer(shader);
	r->setup(&d->meshData, d->vertexMode, d->vertexLocation);
	d->boundingBox = d->meshData.bounds();
	addRenderer(r);
	OWSceneComponent::doInit();
}
