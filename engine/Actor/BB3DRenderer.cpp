#include "BB3DRenderer.h"

void BB3DRenderer::add2DBoundingBox(OWSceneComponent* o)
{
	m2DComponents.push_back(o);
}

void BB3DRenderer::add3DBoundingBox(OWSceneComponent* o)
{
	m3DComponents.push_back(o);
}

int BB3DRenderer::preTick()
{
	return 0;
}

void BB3DRenderer::tick(float dt)
{
}

int BB3DRenderer::postTick()
{
	return 0;
}

void BB3DRenderer::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const glm::vec3& cameraPos,
	RenderTypes::ShaderMutator renderCb,
	RenderTypes::ShaderResizer resizeCb)
{
	for (OWSceneComponent* a : m3DComponents)
	{
		a->render(proj, view, model, cameraPos, renderCb, resizeCb);
	}
	for (OWSceneComponent* a : m2DComponents)
	{
		a->render(proj, view, model, cameraPos, renderCb, resizeCb);
	}

}

