#include "ResizeHelper.h"


ResizeHelper::ResizeHelper()
{
	mAspectRatio = 1.0f;// theApp->physicalWindowSize().x / (theApp->physicalWindowSize().y * 1.0f);

	auto cb = std::bind(&ResizeHelper::resizeCallback, this,
		std::placeholders::_1, std::placeholders::_2);
	theApp->addWindowResizeListener(cb, this);
}

void ResizeHelper::resizeCallback(
	GLApplication::WindowResizeType resizeType, glm::ivec2 dimensions)
{
	// There is a lot of interaction here between:
	// 1. mAspectRatio.
	// 2. call to setViewport in Movie.cpp
	// 3. camera projection call passed to render.
	mAspectRatio = dimensions.x / (dimensions.y * 1.0f);
	mAspectRatioModified = true;
}

glm::vec2 ResizeHelper::scaleByAspectRatio(const glm::vec2& toScale) const
{
	glm::vec2 retval = toScale;
	if (mAspectRatio < 1)
	{
		//retval.x *= mAspectRatio;
		//retval.y *= mAspectRatio;
	}
	else
	{
		//retval.x /= mAspectRatio;
		//retval.y /= mAspectRatio;
	}
	return retval;
}