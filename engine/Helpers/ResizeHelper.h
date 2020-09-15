#pragma once

#include "../Core/ListenerHelper.h"
#include "../Core/GLApplication.h"

class ResizeHelper : public ListenerHelper
{
public:
	ResizeHelper();
protected:
	void resizeCallback(GLApplication::WindowResizeType resizeType,
		glm::ivec2 dimensions);
	float aspectRatio() const { return mAspectRatio; }
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale) const;
	float aspectRatioAndInvalidate() const 
	{ 
		mAspectRatioModified = false;
		return mAspectRatio;
	}
	bool aspectRatioModified() const { return mAspectRatioModified;  }
private:
	float mAspectRatio = 1.0f;
	mutable bool mAspectRatioModified = true;
};

