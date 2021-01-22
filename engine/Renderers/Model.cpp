#include "Model.h"

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
	const glm::mat4& model, const MoveController* mover,
	RenderCallbackType renderCb, ResizeCallbackType resizeCb) const
{

	if (!resizeCb)
	{
		// resize all in source->mRootNode
	}
	else
	{

	}

	if (renderCb)
	{
		renderCb(proj, view, model, mRootNode);
	}
	else
	{
		mRootNode.traverse([&](const ModelData* m) {
			for (auto& r : m->renderers)
				r->render(proj, view, model, nullptr, nullptr, resizeCb);
		});
	}
}
