#include "VertexSource.h"

#include <glm/ext/matrix_transform.hpp>

#include "../Helpers/MoveController.h"

#include "VertexSourceRenderer.h"

void VertexSource::addRenderer(VertexSourceRenderer* source)
{
	mRenderer = source;
	mRenderer->prepare(this);
}

void VertexSource::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const MoveController* mover,
	OWUtils::RenderCallbackType renderCb,
	OWUtils::ResizeCallbackType resizeCb) const
{
	if (mover)
	{
		mRenderer->render(this, proj, view, mover->translate(model), renderCb, resizeCb);
	}
	else
	{
		glm::mat4 initialPositionModel = glm::translate(model, initialPosition());
		mRenderer->render(this, proj, view, initialPositionModel, renderCb, resizeCb);
	}
}
