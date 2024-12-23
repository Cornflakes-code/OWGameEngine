#include "OWSceneComponent.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "OWActor.h"

OWSceneComponent::OWSceneComponent(OWActor* _owner, const glm::vec3& _position)
	: OWMovableComponent(_owner, _position) 
{
	_owner->addSceneComponent(this);
}

void OWSceneComponent::scale(const glm::vec3& factor)
{
	mScale *= factor;
}

constexpr float M_TWO_PI = 2 * M_PI;
void OWSceneComponent::rotate(float degrees, const glm::vec3& factor)
{
	mRotateFactor *= factor;
	mRotateRadians += glm::radians(degrees);
	if (mRotateRadians > M_TWO_PI)
		mRotateRadians -= M_TWO_PI;
	else if (mRotateRadians < -M_TWO_PI)
		mRotateRadians += M_TWO_PI;
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
	{
		glm::vec3 p = position();
		glm::vec3 ss = mScale * mScale;
		glm::mat4 i(1.0f);
		s = "Plane1";
	}
	if (s == "Text:X")
		s = "Text:X";
	if (!mReadyForRender)
	{
		throw NMSLogicException("Component: [" + name() + "] not ready for render.");
	}
	if (mRenderThis)
	{
		const glm::mat4 I(1.0f);
		glm::mat4 r = glm::rotate(model, mRotateRadians, mRotateFactor);
		glm::mat4 s = glm::scale(model, mScale);
		glm::mat4 t = glm::translate(I, position());
		glm::mat4 _model = t * s * r;
		mRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);
	}
}
