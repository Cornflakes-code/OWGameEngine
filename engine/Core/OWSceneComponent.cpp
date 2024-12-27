#include "OWSceneComponent.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "CommonUtils.h"
#include "../Renderers/VAOBuffer.h"
#include "../Helpers/Shader.h"

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
	RenderTypes::ShaderResizer resizeCb) 
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
		if (false)//mRenderBoundingBox)
		{
			std::vector<std::vector<glm::vec3>> surfaces = bounds().surfaces();
			if (mRotateRadians > 0)
			{
				mRotateRadians = mRotateRadians;
			}
			if (mBoundingBoxRenderer == nullptr)
			{
				Shader* lineShader = new Shader();
				lineShader->loadBoilerPlates();
				lineShader->setStandardUniformNames("pvm");

				mBoundingBoxRenderer = new VAOBuffer(lineShader, VAOBuffer::DRAW_MULTI);

				for (const std::vector<glm::vec3>& p : surfaces)
				{
					MeshDataLight lineData;
					lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN), "colour");
					lineData.vertices(p, GL_LINE_STRIP);
					mBoundingBoxRenderer->add(&lineData);
				}
				mBoundingBoxRenderer->prepare();
			}
			std::vector<glm::vec3> corners;
			for (const std::vector<glm::vec3>& surf : surfaces)
			{
				for (const glm::vec3& pt : surf)
				{
					glm::vec4 rotatedPoint = _model * glm::vec4(pt, 0);
					corners.push_back(rotatedPoint);
				}
			}
			AABB jfw = bounds();

			// now find the bounding box of the rotated original AABB
			AABB b2(corners);
			float x = glm::abs(((b2.maxPoint().x - b2.minPoint().x)) / (bounds().maxPoint().x - bounds().minPoint().x));
			float y = glm::abs(((b2.maxPoint().y - b2.minPoint().y)) / (bounds().maxPoint().y - bounds().minPoint().y));
			float z = glm::abs(((b2.maxPoint().z - b2.minPoint().z)) / (bounds().maxPoint().z - bounds().minPoint().z));
			glm::vec3 newScaling = glm::vec3(x, y, z);
			//_model = glm::translate(_model, -position());
			//_model = glm::scale(_model, newScaling);
			//_model = glm::translate(_model, position());

			//std::vector<glm::vec3> v;
			//v.push_back({ tmin.x, tmin.y, tmin.z });
			//v.push_back({ tmax.x, tmax.y, tmax.z });
			//bounds(AABB(v));
			mBoundingBoxRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);
		}
	}
}
