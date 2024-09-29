#include "Model.h"

void Model::append(ModelData* md)
{
	for (auto& a : md->children)
	{
		mRootNode.children.push_back(a);
	}
	for (auto& a : md->meshes)
	{
		mRootNode.meshes.push_back(a);
	}
	for (auto& a : md->renderers)
	{
		mRootNode.renderers.push_back(a);
	}
	mRootNode.traverse([&](ModelData* m) {
		if (m->renderers.size() < m->meshes.size())
		{
			throw NMSLogicException("All meshes in the model must have a renderer.");
		}
		});
}

void Model::setup(ModelData* md)
{
	mRootNode = *md;
	mRootNode.traverse([&](ModelData* m) {
		if (m->renderers.size() < m->meshes.size())
		{
			throw NMSLogicException("All meshes in the model must have a renderer.");
		}
		});
}

void Model::render(const glm::mat4& proj, const glm::mat4& view,
	const glm::mat4& model, const glm::vec3& cameraPos,
	MoveController* mover, 
	RendererBase::RenderCallbackType renderCb,
	RendererBase::ResizeCallbackType resizeCb) const
{
/*
	if (!resizeCb)
	{
		// resize all in source->mRootNode
	}
	else
	{

	}

	if (renderCb)
	{
		renderCb(proj, view, model, cameraPos, mRootNode[0].shader());
	}
*/
	mRootNode.traverse([&](const ModelData* m) {
		for (auto& r : m->renderers)
			r->render(proj, view, model, cameraPos, mover, renderCb, resizeCb);
	});
}
