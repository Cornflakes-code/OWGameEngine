#include "TextBillboardDynamic.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Helpers/Shader.h"

//http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/

TextBillboardDynamic::TextBillboardDynamic(const glm::vec3& initialPosition,
				const std::string& fontFileName, int fontHeight)
	:TextBillboard(initialPosition, fontFileName, fontHeight)
{
	shader(new Shader("textDynamicBillboard.v.glsl", "text.f.glsl", ""), "VP");
	mVertexLoc = mShader->getAttributeLocation("coord");
	mVertexMode = GL_TRIANGLES;

	mRenderCallback
		= std::bind(&TextBillboardDynamic::renderCallback,
			this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3, std::placeholders::_4);
	mResizeCallback
		= std::bind(&TextBillboardDynamic::resizeCallback,
			this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3);
}

void TextBillboardDynamic::renderCallback(
			const glm::mat4& proj, const glm::mat4& view,
			const glm::mat4& model, Shader* shader)
{
	const glm::mat4 pv = proj * view;
	mShader->use();
	mShader->setMatrix4("VP", pv);

	glm::vec3 CameraRight_worldspace =
	{ view[0][0], view[1][0], view[2][0] };
	mShader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
	glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
	mShader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
	glm::vec4 center = bounds().center();
	glm::mat4 newModel = glm::translate(model, glm::vec3(center.x, center.y, center.z));
	glm::vec3 xx = newModel[3];

	mShader->setVector3f("BillboardPos", newModel[3]);
}

void TextBillboardDynamic::resizeCallback(Shader* shader,
	OWUtils::ScaleByAspectRatioType scaleByAspectRatio,
					float aspectRatio)
{
	glm::vec2 bbSize({ bounds().size().x * mScale.x, bounds().size().y * mScale.y });
	mShader->setVector2f("BillboardSize", bbSize);
}
