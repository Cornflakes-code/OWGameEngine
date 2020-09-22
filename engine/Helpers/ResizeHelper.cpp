#include "ResizeHelper.h"


ResizeHelper::ResizeHelper()
: mAspectRatio(1.0f)// globals->physicalWindowSize().x / (globals->physicalWindowSize().y * 1.0f);
{
	auto cb = std::bind(&ResizeHelper::resizeCallback, this,
		std::placeholders::_1, std::placeholders::_2);
	globals->application()->addWindowResizeListener(cb, this);
}

void ResizeHelper::resizeCallback(GLFWwindow* window, glm::ivec2 dimensions)
{
	// This is FrameBufferResize callback
	// There is a lot more to do this correctly
	// https://stackoverflow.com/questions/45796287/screen-coordinates-to-world-coordinates
	// https://www.glfw.org/docs/latest/window_guide.html#window_size

	// There is a lot of interaction here between:
	// 1. mAspectRatio.ss
	// 2. call to setViewport in Movie.cpp
	// 3. camera projection call passed to render.
	mAspectRatio = dimensions.x / (dimensions.y * 1.0f);
	mAspectRatioChangedSinceLastRead = true;
}

glm::vec2 ResizeHelper::scaleByAspectRatio(const glm::vec2& toScale, bool clearFlag) const
{
	// This seems to work best (trial and error) when resizing the window.
	glm::vec2 retval = toScale;
	if (mAspectRatio < 1)
	{
		retval.x /= mAspectRatio;
		retval.y *= mAspectRatio;
	}
	else
	{
		retval.x /= mAspectRatio;
		//retval.y *= mAspectRatio;
	}
	if (clearFlag)
		mAspectRatioChangedSinceLastRead = false;
	return retval;
}