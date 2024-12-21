#include "OWSceneComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "OWActor.h"

OWSceneComponent::OWSceneComponent(OWActor* _owner, const glm::vec3& _position)
	: OWMovableComponent(_owner, _position) 
{
	_owner->addSceneComponent(this);
}

void OWSceneComponent::scale(const glm::vec3& factor)
{
	mModelMatrix = glm::scale(mModelMatrix, factor);
}

void OWSceneComponent::translate(const glm::vec3& factor)
{
	mModelMatrix = glm::translate(mModelMatrix, factor);
}

void OWSceneComponent::rotate(float angle, const glm::vec3& factor)
{
	mModelMatrix = glm::rotate(mModelMatrix, angle, factor);
}


void OWSceneComponent::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const glm::vec3& cameraPos,
	RenderTypes::ShaderMutator renderCb,
	RenderTypes::ShaderResizer resizeCb) const 
{
	std::string s = name();
	if (s == "Plane1")
		s = "Plane1";
	if (!mReadyForRender)
	{
		throw NMSLogicException("Component: [" + name() + "] not ready for render.");
	}
	if (mRenderThis)
	{
		glm::mat4 _model = mModelMatrix * model;
		mRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);
	}
}
