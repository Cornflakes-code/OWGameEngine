#include "TextBillboardFixed.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Helpers/Shader.h"

TextBillboardFixed::TextBillboardFixed(const glm::vec3& initialPosition,
		const std::string& fontFileName, int fontHeight)
:TextBillboard(initialPosition, fontFileName, fontHeight)
{
	// The following has some info
	// http://ogldev.org/www/tutorial27/tutorial27.html
	shader(new Shader("textStaticBillboard.v.glsl", "text.f.glsl", ""), "VP");
	mShader->use();
	mVertexLoc = mShader->getAttributeLocation("coord");
	mVertexMode = GL_TRIANGLES;

	mRenderCallback
		= std::bind(&TextBillboardFixed::renderCallback,
			this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3, std::placeholders::_4);
	mResizeCallback
		= std::bind(&TextBillboardFixed::resizeCallback,
			this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3);
}

void TextBillboardFixed::renderCallback(
		const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, const Shader* shader)
{
	glm::vec4 center = bounds().center();
	glm::mat4 newModel =
		glm::translate(model, glm::vec3(center.x, center.y, center.z));
	shader->setVector3f("BillboardPos", newModel[3]);
}

void TextBillboardFixed::resizeCallback(const Shader* shader,
			OWUtils::ScaleByAspectRatioType scaleByAspectRatio,
			float aspectRatio)
{
	glm::vec2 vv = { 0.5, 0.5 };
	glm::vec2 v2 = scaleByAspectRatio(vv);
	shader->setVector2f("BillboardSize", v2);
}
