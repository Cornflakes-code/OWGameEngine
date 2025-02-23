#include "MeshComponentLight.h"

#include <Renderers/LightRenderer.h>

void MeshComponentLight::doInit()
{
	MeshComponentLightData* d = data();
	d->shaderData.projectionName = "";
	d->shaderData.viewName = "";
	d->shaderData.modelName = "";
	Shader* shader = new Shader(&d->shaderData);
	LightRenderer* r = new LightRenderer(shader);
	r->setup(d->meshData);
	d->boundingBox = d->meshData.bounds();
	addRenderer(r);
	OLDSceneComponent::doInit();
}
