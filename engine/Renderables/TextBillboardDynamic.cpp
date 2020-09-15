#include "TextBillboardDynamic.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Helpers/Shader.h"

TextBillboardDynamic::TextBillboardDynamic(const std::string& fontFileName, int fontHeight)
	:TextBillboard(new Shader("textDynamicBillboard.v.glsl", "text.f.glsl", ""),
		fontFileName, fontHeight)
{
}

void TextBillboardDynamic::doRender(const glm::mat4& proj, 
									const glm::mat4& view, 
									const glm::mat4& model) const
{
	const glm::mat4 pv = proj * view;
	mShader->use();
	mShader->setMatrix4("VP", pv);

	glm::vec3 CameraRight_worldspace =
	{ view[0][0], view[1][0], view[2][0] };
	mShader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
	glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
	mShader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
	glm::vec4 center = mBounds.center();
	glm::mat4 newModel = glm::translate(model, glm::vec3(center.x, center.y, center.z));
	glm::vec3 xx = newModel[3];

	mShader->setVector3f("BillboardPos", newModel[3]);
	if (aspectRatioModified())
	{
		float ratio = aspectRatioAndInvalidate();
		glm::vec2 scale = scaleByAspectRatio(mScale);
		glm::vec2 bbSize({ mBounds.size().x / scale.x, mBounds.size().y / scale.y });
		mShader->setVector2f("BillboardSize", bbSize);
	}
	mShader->setVector4f("textcolor", mColor);
}
