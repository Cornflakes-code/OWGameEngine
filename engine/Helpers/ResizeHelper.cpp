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
	if (resizeType == GLApplication::WindowResizeType::FrameBuffer)
	{
		// There is a lot more to do this correctly
		// https://stackoverflow.com/questions/45796287/screen-coordinates-to-world-coordinates

		// There is a lot of interaction here between:
		// 1. mAspectRatio.ss
		// 2. call to setViewport in Movie.cpp
		// 3. camera projection call passed to render.
		mAspectRatio = dimensions.x / (dimensions.y * 1.0f);
		mAspectRatioChangedSinceLastRead = true;
	}
	else if (resizeType == GLApplication::WindowResizeType::WindowResize)
	{
		// Do not pass the window size to glViewport or other pixel-based OpenGL calls.
		// The window size is in screen coordinates, not pixels. Use the framebuffer 
		// size, which is in pixels, for pixel - based calls.
		// https://www.glfw.org/docs/latest/window_guide.html#window_size
	}
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