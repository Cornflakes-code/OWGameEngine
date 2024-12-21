#include "MeshComponent.h"

#include <Renderers/HeavyRenderer.h>
#include <Renderers/LightRenderer.h>
#include <Renderers/InstanceRenderer.h>
#include <Renderers/VAOBuffer.h>

void MeshComponent::setup(const MeshDataHeavy* md, Shader* shader, unsigned int vertexMode, unsigned int vertexLocation)
{
	HeavyRenderer* r = new HeavyRenderer(shader);
	r->setup(md, vertexMode, vertexLocation);
	bounds(md->bounds());
	addRenderer(r);
	readyForRender();
}

void MeshComponent::setup(const MeshDataLight* md, Shader* shader)
{
	LightRenderer* r = new LightRenderer(shader);
	r->setup(md);
	bounds(md->bounds());
	addRenderer(r);
	readyForRender();
}

void MeshComponent::setup(const std::vector<glm::vec4>& v, Shader* shader,
	unsigned int vertexMode, unsigned int vertexLocation)
{
	LightRenderer* r = new LightRenderer(shader);
	r->setup(v, vertexMode, vertexLocation);
	bounds(convertToV3(v));
	addRenderer(r);
	readyForRender();
}

void MeshComponent::setup(const MeshDataInstance* md, Shader* shader)
{
	InstanceRenderer* r = new InstanceRenderer(shader);
	r->setup(md);
	mCanCollide = false;
	addRenderer(r);
	readyForRender();
}

void MeshComponent::setup(VAOBuffer* vao, Shader* shader)
{
	if (shader)
		vao->shader(shader);
	vao->prepare();
	addRenderer(vao);
	readyForRender();
}

