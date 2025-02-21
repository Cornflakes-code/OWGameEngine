#include "TextComponent.h"

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderers/TextRendererDynamic.h"
#include "../Renderers/TextRendererStatic.h"
#include "../Renderers/VAOBuffer.h"
#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"

TextComponent::TextComponent(OWActor* _owner, TextComponentData* _data)
	: OWSceneComponent(_owner, _data)
{
}

void TextComponent::doInit()
{
	TextData* jfw = textData();
	name("Text:" + textData()->text);

	// If both left and right then remove them
	if (((textData()->referencePos & TextData::PositionType::Left) &&
		(textData()->referencePos & TextData::PositionType::Right)))
	{
		textData()->referencePos = TextData::PositionType(textData()->referencePos & 0xC);
	}

	// If both Top and Bottom then remove them
	if (((textData()->referencePos & TextData::PositionType::Top) &&
		(textData()->referencePos & TextData::PositionType::Bottom)))
	{
		textData()->referencePos = TextData::PositionType(textData()->referencePos & 0x3);
	}
	TextRenderer* ren;
	if (textData()->tdt == TextData::TextDisplayType::Dynamic)
		ren = new TextRendererDynamic();
	else
		ren = new TextRendererStatic();
	ren->setup(this, glm::vec3(0));
	data()->boundingBox = ren->bounds();
	addRenderer(ren);
	OWSceneComponent::doInit();
}

AABB calcOrthogonalAABB(const glm::vec3& right, const glm::vec3& up,
	const glm::vec3& inOut, const AABB& orig, float distanceToEye)
{
	if (OWUtils::isZero(distanceToEye))
		distanceToEye = 0.001f;
	float dist = 20000 / distanceToEye;
	std::vector<glm::vec3> v;
	glm::vec3 sz = orig.size() * 0.5f; // mult by half extent.
	v.push_back(sz * right); //rhs
	v.push_back(-sz * right); // lhs
	v.push_back(sz * up); // up
	v.push_back(-sz * up); // down
	v.push_back(-sz * inOut); // in
	v.push_back(sz * inOut); // out
	for (glm::vec3& a : v)
	{
		a.x *= dist;
		a.y *= dist;
	}
	AABB bb(v);
	return bb;
}

void TextComponent::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const glm::vec3& cameraPos,
	RenderTypes::ShaderMutator renderCb,
	RenderTypes::ShaderResizer resizeCb)
{
	std::string ss = name();
	if (ss == "Rope Ends")
	{
		ss = "Rope Ends";
	}
	if (data()->physics.visibility > 0.001f)
	{
		OWPhysicsDataImp* imp = &data()->physics;
		const glm::mat4 I(1.0f);
		float len = glm::length(imp->mRotate);
		glm::vec3 norm = glm::normalize(imp->mRotate);
		glm::mat4 r1 = OWUtils::isZero(len) ? I : glm::rotate(model, len, norm);
		glm::mat4 s1 = glm::scale(I, imp->mScale);
		glm::mat4 t1 = glm::translate(I, imp->mTranslate);
		glm::mat4 _model = t1 * r1 * s1;
		mRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);
		if (mBoundingBoxRenderer != nullptr)
		{
			if (ss == "Text:Enjoy it while you can.")
			{
				const AABB& orig = mOriginalBoundingBox;
				glm::vec3 center = orig.center();
				glm::vec3 orient = glm::normalize(cameraPos - center);
				glm::vec3 right = glm::normalize(glm::vec3(view[0][0], view[1][0], view[2][0]));
				glm::vec3 up = glm::normalize(glm::cross(orient, right));
				//glm::vec3 up = glm::normalize(glm::vec3(view[0][1], view[1][1], view[2][1]));
				AABB bb = calcOrthogonalAABB(right, up, orient, orig, glm::length(cameraPos - center));
				bb.scale(scale());
				bb.move(data()->physics.mTranslate + center);
				data()->boundingBox = bb;
				
			}
			else if (ss == "Text:Welcome to reality.")
			{
				glm::vec3 right = glm::normalize(glm::vec3(view[0][0], view[1][0], view[2][0]));
				glm::vec3 up = glm::normalize(glm::vec3(view[0][1], view[1][1], view[2][1]));
				const AABB& orig = mOriginalBoundingBox;
				glm::vec3 center = orig.center();
				glm::vec3 inOut = glm::normalize(cameraPos - center);
				AABB bb = calcOrthogonalAABB(right, up, inOut, orig, 100.0f);
				bb.scale(scale());
				bb.move(data()->physics.mTranslate + center);
				data()->boundingBox = bb;
				/*
				glm::vec3 right = { view[0][0], view[1][0], view[2][0] };
				glm::vec3 up = { view[0][1], view[1][1], view[2][1] };
				const AABB& orig = mOriginalBoundingBox;
				const AABB& current = data()->boundingBox;
				glm::mat4 model2(0);
				glm::vec3 newScaling(0);
				model2[0][0] = right[0];
				model2[1][0] = right[1];
				model2[2][0] = right[2];
				model2[0][1] = right[2];
				model2[1][1] = right[2];
				model2[2][1] = right[2];
				glm::vec3 t = translation() + orig.center();
				model2[3][0] = t[0];
				model2[3][1] = t[1];
				model2[3][2] = t[2];
				model2[3][3] = 1;
				newScaling.x = glm::abs(((current.maxPoint().x - current.minPoint().x)) / (orig.maxPoint().x - orig.minPoint().x));
				newScaling.y = glm::abs(((current.maxPoint().y - current.minPoint().y)) / (orig.maxPoint().y - orig.minPoint().y));
				newScaling.z = glm::abs(((current.maxPoint().z - current.minPoint().z)) / (orig.maxPoint().z - orig.minPoint().z));
				glm::vec2 sz = orig.size() * scale();
				// These point s are the first texture entry in RenderDoc.
				//glm::vec4 coord = { -6.28125, 1.3125, 0.38933, 0.32836 };
				glm::vec3 minPos = t
					+ right * orig.minPoint().x * sz.x
					+ up * orig.minPoint().y * sz.y;
				//minPos = proj * view * glm::vec4(minPos, 1.0f);

				glm::vec3 maxPos = t
					+ right * orig.maxPoint().x * sz.x
					+ up * orig.maxPoint().y * sz.y;
				//maxPos = proj * view * glm::vec4(maxPos, 1.0f);

				std::vector<glm::vec3> v;
				v.push_back(minPos);
				v.push_back(maxPos);
				AABB bb(v);
				data()->boundingBox = bb;
				//const glm::mat4 I2 = glm::inverse(model);
				//glm::mat4 s2 = glm::scale(I2, sz);
				//glm::mat4 t2 = glm::translate(I2, glm::vec3(translation()));
				//glm::mat4 model2 = r * s * t;
				glm::mat4 ortho = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
				*/
				//mBoundingBoxRenderer->render(proj, view, model2, cameraPos, renderCb, resizeCb);
				mBoundingBoxRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);
			}
			else
			{
			}
		}
	}
}

