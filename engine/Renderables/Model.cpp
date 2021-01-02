#include "Model.h"

#include <glm/ext/matrix_transform.hpp>

//#include "../Helpers/MoveController.h"

#include "../Helpers/ModelData.h"
#include "../Helpers/MoveController.h"
#include "../Helpers/ModelFactory.h"

#include "ModelRenderer.h"
#include "Mesh.h"

void Model::create(const std::string& modelFileName, bool cache)
{
	mRootNode = ModelFactory().create(modelFileName, cache);
	glm::vec3 pos = initialPosition();
	mRootNode->dfsTraverse([&pos](ModelData* m) {
		for (auto& mesh : m->meshes)
		{
			mesh.initialPosition(pos);
		}
	});
}

void Model::renderer(ModelRenderer* source)
{
	mRenderer = source;
	mRenderer->prepare(this);
}

AABB Model::calcBounds() const
{
	return AABB();
}

void Model::render(const glm::mat4& proj,
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
