#include "MeshComponentLight.h"

#include <Renderers/LightRenderer.h>

void MeshComponentLight::init()
{
	MeshComponentLightData* d = data();
	Shader* shader = new Shader(&d->shaderData);
	LightRenderer* r = new LightRenderer(shader);
	r->setup(d->meshData);
	d->boundingBox = d->meshData.bounds();
	addRenderer(r);
}
