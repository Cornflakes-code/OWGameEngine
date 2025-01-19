#include "OWSceneComponent.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderers/VAOBuffer.h"

#include "OWActor.h"

OWSceneComponent::OWSceneComponent(OWActor* _owner, const glm::vec3& _position)
	: OWMovableComponent(_owner, _position) 
{
	_owner->addSceneComponent(this);
}

void OWSceneComponent::scale(const glm::vec3& factor)
{
	glm::vec3 mi = bounds().minPoint() - bounds().center();
	mi *= factor;
	glm::vec3 ma = bounds().maxPoint() - bounds().center();
	ma *= factor;

	AABB newBounds(mi + bounds().center(), ma + bounds().center());
	bounds(newBounds);
	mScale *= factor;
}

constexpr float M_TWO_PI = 2 * M_PI;
void OWSceneComponent::rotate(float degrees, const glm::vec3& axis)
{
	AABB newBounds = bounds().findBoundsIfRotated(glm::radians(degrees), axis);
	bounds(newBounds);
	mRotateAxis *= axis;
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
	RenderTypes::ShaderResizer resizeCb) 
{
	std::string ss = name();
	if (ss == "Plane3" || ss == "box")
	{
		glm::vec3 p = position();
	}
	if (ss == "Text:Enjoy it while you can")
		ss = "Text:X";
	if (!readyForRender())
	{
		throw NMSLogicException("Component: [" + name() + "] not ready for render.");
	}
	if (mRenderThis)
	{
		const glm::mat4 I(1.0f);
		glm::mat4 r = glm::rotate(model, mRotateRadians, mRotateAxis);
		glm::mat4 s = glm::scale(model, mScale);
		glm::mat4 t = glm::translate(I, position());
		glm::mat4 _model = t * r * s;
		mRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);

		if (mRenderBoundingBox)
		{
			// We need to find how much the current AABB has been scaled from the original.
			// This will include mScale as well as any extra scaling due to rotations (since we do not rotate an AABB).
			glm::vec3 newScaling(0);
			const AABB& orig = boundBoxOriginal();
			const AABB& current = bounds();
			newScaling.x = glm::abs(((current.maxPoint().x - current.minPoint().x)) / (orig.maxPoint().x - orig.minPoint().x));
			newScaling.y = glm::abs(((current.maxPoint().y - current.minPoint().y)) / (orig.maxPoint().y - orig.minPoint().y));
			newScaling.z = glm::abs(((current.maxPoint().z - current.minPoint().z)) / (orig.maxPoint().z - orig.minPoint().z));

			const glm::mat4 I2(1.0f);
			glm::mat4 s2 = glm::scale(model, newScaling);
			glm::mat4 t2 = glm::translate(I2, position());
			glm::mat4 model2 = t2 * s2;
/*
			// http://www.geeks3d.com/20140815/particle-billboarding-with-the-geometry-shader-glsl/
			glm::vec3 right, up;
			right.x = model2[0][0];  // 0
			right.y = model2[1][0];  // 4
			right.z = model2[2][0];  // 8

			up.x = model2[0][1];  // 1
			up.y = model2[1][1];  // 5
			up.z = model2[2][1];  // 9
			float size = 1.0f;
			glm::vec3 a = P - (right + up) * size;
			glm::vec3 b = P - (right - up) * size;
			glm::vec3 d = P + (right - up) * size;
			glm::vec3 c = P + (right + up) * size;
*/
			boundingBoxRenderer()->render(proj, view, model2, cameraPos, renderCb, resizeCb);
		}
	}
}
