#include "MeshActor.h"
#include <Renderers/HeavyRenderer.h>
#include <Renderers/LightRenderer.h>

void MeshActor::setup(const MeshDataHeavy* md, Shader* shader, unsigned int vertexMode, unsigned int vertexLocation)
{
	HeavyRenderer* r = new HeavyRenderer(shader);
	r->setup(md, vertexMode, vertexLocation);
	bounds(md->bounds());
	addRenderer(r);
}

void MeshActor::setup(const MeshDataLight* md, Shader* shader)
{
	LightRenderer* r = new LightRenderer(shader);
	r->setup(md);
	bounds(md->bounds());
	addRenderer(r);
}

void MeshActor::setup(const std::vector<glm::vec4>& v, Shader* shader,
	unsigned int vertexMode, unsigned int vertexLocation)
{
	LightRenderer* r = new LightRenderer(shader);
	r->setup(v, vertexMode, vertexLocation);
	bounds(AABB::calcBounds(v));
	addRenderer(r);
}


