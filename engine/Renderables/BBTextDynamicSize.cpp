#include "BBTextDynamicSize.h"

#include <glm/gtx/rotate_vector.hpp>

#include "../Helpers/Shader.h"

BBTextDynamicSize::BBTextDynamicSize(
	const std::string& fontFileName, int fontHeight)
{
	mFontData = factory()->loadFreeTypeFont(fontFileName, fontHeight);
}

BBTextDynamicSize::~BBTextDynamicSize()
{
	theApp->removeWindowResizeListener(this);
	delete mShader;
}

void BBTextDynamicSize::resizeCallback(
	GLApplication::WindowResizeType resizeType,
	glm::ivec2 dimensions)
{
	// There is a lot of interaction here between:
	// 1. mAspectRatio.
	// 2. call to setViewport in Movie.cpp
	// 3. camera projection call passed to render.
	mAspectRatio = dimensions.x / (dimensions.y * 1.0f);
}

void BBTextDynamicSize::text(const std::string& text, float sx, float sy)
{
	const std::vector<glm::vec4>& temp = mFontData->createText(text, sx, sy);
	mVertices.insert(mVertices.begin(), temp.begin(), temp.end());
}

void BBTextDynamicSize::doRender(const glm::mat4& proj,
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
	if (!OWUtils::isEqual(mAspectRatio, 0.0f))
	{
		glm::vec2 scale = { 0.5, 0.5 };
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
		glm::vec2 bbSize({ mBounds.size().x / scale.x, mBounds.size().y / scale.y });
		mShader->setVector2f("BillboardSize", bbSize);
	}

	mShader->setVector3f("BillboardPos", newModel[3]);
	mShader->setVector4f("textcolor", mColor);
}
