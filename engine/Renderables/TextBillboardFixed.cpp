#include "TextBillboardFixed.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Helpers/Shader.h"

TextBillboardFixed::TextBillboardFixed(const std::string& fileName, int fontHeight)
	:TextBillboard(new Shader("textStaticBillboard.v.glsl", "text.f.glsl", ""),
					fileName, fontHeight)
{
}

void TextBillboardFixed::doRender(const glm::mat4& proj, 
								  const glm::mat4& view, 
								  const glm::mat4& model) const
{
	const glm::mat4 pv = proj * view;
	mShader->use();
	mShader->setMatrix4("VP", pv);

	glm::vec4 center = mBounds.center();
	glm::mat4 newModel = glm::translate(model, glm::vec3(center.x, center.y, center.z));
	glm::vec3 xx = newModel[3];
	mShader->setVector3f("BillboardPos", newModel[3]);
	if (aspectRatioModified())
	{
		glm::vec2 scale = scaleByAspectRatio(mScale);
		mShader->setVector2f("BillboardSize", scale);
	}
	mShader->setVector4f("textcolor", mColor);
}
