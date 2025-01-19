#include "TextData.h"

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderers/TextRendererDynamic.h"
#include "../Renderers/TextRendererStatic.h"
#include "../Renderers/VAOBuffer.h"
#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"

void TextData::prepare()
{
	name("Text:" + mText);

	TextRenderer* ren;
	if (mDynamicSize == Dynamic)
		ren = new TextRendererDynamic();
	else
		ren = new TextRendererStatic();
	ren->setup(this, glm::vec3(0));
	bounds(ren->bounds());
	addRenderer(ren);
	readyForRender(true);
}

void TextData::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const glm::vec3& cameraPos,
	RenderTypes::ShaderMutator renderCb,
	RenderTypes::ShaderResizer resizeCb)
{
	//OWSceneComponent::render(proj, view, model, cameraPos, renderCb, resizeCb);
	//return;
	std::string ss = name();
	if (ss == "Plane3" || ss == "box")
	{
		glm::vec3 p = position();
	}
	if (ss == "Text:Enjoy it while you can.")
		ss = "Text:Enjoy it while you can.";
	if (!readyForRender())
	{
		throw NMSLogicException("Component: [" + name() + "] not ready for render.");
	}
	if (mRenderThis)
	{
		const glm::mat4 I(1.0f);
		glm::mat4 r = glm::rotate(model, mRotateRadians, mRotateAxis);
		glm::mat4 s = glm::scale(model, scale());
		glm::mat4 t = glm::translate(I, position());
		glm::mat4 _model = t * r * s;
		mRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);
		if (mRenderBoundingBox)
		{
			if (ss == "Text:Enjoy it while you can.")
			{
				// static
				AABB orig = boundBoxOriginal();
				glm::vec3 sz = orig.size() * 10.0f;
				glm::vec2 half(globals->physicalWindowSize().x / 2.0f, globals->physicalWindowSize().y / 2.0f);
				glm::mat4 ortho = glm::ortho(-half.x, half.x, half.y, -half.y, -1.0f, 1.0f);
				//glm::vec4 BillboardPos = glm::vec4(0.021875, 0.05625, 0.179061, 1.0);
				glm::vec3 bbsize = sz;
				glm::mat4 vp = proj * view;
				glm::vec3 p3 = position();
				glm::vec4 sz4 = vp * glm::vec4(sz.x, sz.y, sz.z, 1.0f);
				//sz4 = vp * glm::vec4(sz, 0.0f);
				glm::vec4 gl_pos = vp * glm::vec4(p3.x, p3.y, p3.z, 1.0f);//;// BillboardPos;
				gl_pos /= gl_pos.w;
				sz4 /= gl_pos.w;
				glm::vec3 minPos = glm::vec3(gl_pos.x, gl_pos.y, gl_pos.z) + orig.minPoint() * glm::vec3(sz4);
				minPos.z = -1;
				glm::vec3 maxPos = glm::vec3(gl_pos.x, gl_pos.y, gl_pos.z) + orig.maxPoint() * glm::vec3(sz4);
				maxPos.z = 1;
				glm::mat4 orthoI = glm::inverse(ortho);
				AABB bb(minPos, maxPos);
				
				bounds(bb);

				const glm::mat4 I2(1.0f); // glm::inverse(model);
				glm::mat4 s2 = glm::scale(I2, sz * scale());
				glm::mat4 t2 = glm::translate(I2, glm::vec3(position()));
				glm::mat4 model2 = t2 * s2;

				boundingBoxRenderer()->render(proj, view, model2, cameraPos, renderCb, resizeCb);
				return;
			}
			else if (ss == "Text:Welcome to reality.")
			{
				glm::vec3 right = { view[0][0], view[1][0], view[2][0] };
				glm::vec3 up = { view[0][1], view[1][1], view[2][1] };
				AABB orig = boundBoxOriginal();

				glm::vec3 sz = orig.size() * scale();
				glm::vec3 minPos = position()
					+ right * orig.minPoint().x * sz.x
					+ up * orig.minPoint().y * sz.y;

				glm::vec3 maxPos = position()
					+ right * orig.maxPoint().x * sz.x
					+ up * orig.maxPoint().y * sz.y;
				AABB bb(minPos, maxPos);
				bounds(bb);
				const glm::mat4 I2(1.0f); // glm::inverse(model);
				glm::mat4 s2 = glm::scale(I2, sz);
				glm::mat4 t2 = glm::translate(I2, glm::vec3(position()));
				glm::mat4 model2 = t2 * s2;
				boundingBoxRenderer()->render(proj, view, model2, cameraPos, renderCb, resizeCb);
				return;
			}
			// We need to find how much the current AABB has been scaled from the original.
			// This will include mScale as well as any extra scaling due to rotations (since we do not rotate an AABB).
			glm::vec3 newScaling(0);
			const AABB& orig = boundBoxOriginal();
			const AABB& current = bounds();
			newScaling.x = glm::abs(((current.maxPoint().x - current.minPoint().x)) / (orig.maxPoint().x - orig.minPoint().x));
			newScaling.y = glm::abs(((current.maxPoint().y - current.minPoint().y)) / (orig.maxPoint().y - orig.minPoint().y));
			newScaling.z = glm::abs(((current.maxPoint().z - current.minPoint().z)) / (orig.maxPoint().z - orig.minPoint().z));
			newScaling = glm::vec3(1, 1, 1);
			const glm::mat4 I2(1.0f);
			glm::mat4 s2 = glm::scale(model, newScaling);
			glm::mat4 t2 = glm::translate(I2, position());
			glm::mat4 model2 = t2 * s2;

			boundingBoxRenderer()->render(proj, view, model2, cameraPos, renderCb, resizeCb);
		}
	}
}
