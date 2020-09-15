#include "BBTextFixedSize.h"

#include <glm/gtx/rotate_vector.hpp>

#include "../Helpers/Shader.h"

BBTextFixedSize::BBTextFixedSize(
				const std::string& fontFileName, int fontHeight)
{
	mFontData = factory()->loadFreeTypeFont(fontFileName, fontHeight);
}

BBTextFixedSize::~BBTextFixedSize()
{
	theApp->removeWindowResizeListener(this);
	delete mShader;
}

void BBTextFixedSize::resizeCallback(
							GLApplication::WindowResizeType resizeType, 
							glm::ivec2 dimensions)
{
	// There is a lot of interaction here between:
	// 1. mAspectRatio.
	// 2. call to setViewport in Movie.cpp
	// 3. camera projection call passed to render.
	mAspectRatio = dimensions.x / (dimensions.y * 1.0f);
}

void BBTextFixedSize::text(const std::string& text, float sx, float sy)
{
	const std::vector<glm::vec4>& temp = mFontData->createText(text, sx, sy);
	mVertices.insert(mVertices.begin(), temp.begin(), temp.end());
}

void BBTextFixedSize::doRender(const glm::mat4& proj,
							const glm::mat4& view,
							const glm::mat4& model) const 
{
	mShader->use();
	mShader->setMatrix4("VP", proj * view);

	glm::vec4 center(1.0);// = mBounds.center();
	glm::mat4 newModel = glm::translate(model, glm::vec3(center.x, center.y, center.z));
	mShader->setVector3f("BillboardPos", newModel[3]);
	glm::vec2 scale = { 0.5, 0.5};
	if (!OWUtils::isEqual(mAspectRatio, 0.0f))
	{
		if (mAspectRatio > 1)
		{
			scale.x /= mAspectRatio;
			scale.y *= mAspectRatio;
		}
		else
		{
			scale.x *= mAspectRatio;
			scale.y /= mAspectRatio;
		}
		mAspectRatio = 1.0f;
	}
	mShader->setVector2f("BillboardSize", scale);
	mShader->setVector4f("textcolor", mColor);
}
