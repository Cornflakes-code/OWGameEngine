#pragma once

#include "../Core/ListenerHelper.h"
#include "../Core/GLApplication.h"

class OWENGINE_API ResizeHelper : public ListenerHelper
{
public:
	ResizeHelper();
protected:
	void resizeCallback(GLApplication::WindowResizeType resizeType,
		glm::ivec2 dimensions);
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale, 
					bool clearFlag = true) const;
	float aspectRatio(bool clearFlag = true) const
	{ 
		if (clearFlag)
			mAspectRatioChangedSinceLastRead = false;
		return mAspectRatio;
	}
	bool aspectRatioModified() const { return mAspectRatioChangedSinceLastRead;  }
private:
	float mAspectRatio = 1.0f;
	mutable bool mAspectRatioChangedSinceLastRead = true;
};

