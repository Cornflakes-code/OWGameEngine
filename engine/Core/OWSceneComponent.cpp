#include "OWSceneComponent.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "CommonUtils.h"
#include "../Renderers/VAOBuffer.h"
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

glm::mat4 transformHelper(const glm::mat4& model, float rotAngle, const glm::vec3& rotFactor, const glm::vec3& scale, const glm::vec3& pos)
{
	const glm::mat4 I(1.0f);
	glm::mat4 r = glm::rotate(model, rotAngle, rotFactor);
	glm::mat4 s = glm::scale(model, scale);
	glm::mat4 t = glm::translate(I, pos);
	return t * s * r;
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
		glm::mat4 _model = transformHelper(model, mRotateRadians, mRotateFactor, mScale, position());
		AABB bb = bounds();
		mRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);
		if (mRenderBoundingBox)
		{
			if (mBoundingBoxRenderer == nullptr)
			{
				/*
				glm::mat4 I(1.0f);
				const glm::vec4 tmin = glm::vec4(bb.minPoint(), 0);
				const glm::mat4 tmin4 = I * tmin;
				glm::mat4 tmin = transformHelper(tmin, mRotateRadians, mRotateFactor, mScale, position());
				glm::mat4 tmax = transformHelper(bb.maxPoint(), mRotateRadians, mRotateFactor, mScale, position());
				AABB newBounds(= bb * model);
				MeshDataLight lineData;
				lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN), "colour");
				lineData.vertices(aLine, GL_LINE_STRIP);
				vao->add(&lineData);
				*/
			}
		}
	}
}
