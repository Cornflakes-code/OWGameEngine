#pragma once

#include "../Core/ListenerHelper.h"
#include "../Core/GLApplication.h"

class Shader;
class OWENGINE_API ResizeHelper : public ListenerHelper
{
public:
	//typedef glm::vec2(ResizeHelper::* ScaleByAspectRatioType)(const glm::vec2&) const;
	typedef std::function< glm::vec2(const glm::vec2)> ScaleByAspectRatioType;
	typedef std::function<void(Shader* shader,
				ScaleByAspectRatioType scaler,
				float aspectRatio)> ResizeCallbackType;

	ResizeHelper();
protected:
	void resizeCallback(GLFWwindow* window, glm::ivec2 dimensions);
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale) const;
	float aspectRatio(bool clearFlag = true) const
	{ 
		if (clearFlag)
			mAspectRatioChangedSinceLastRead = false;
		return mAspectRatio;
	}
	bool aspectRatioModified() const { return mAspectRatioChangedSinceLastRead;  }
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	float mAspectRatio = 1.0f;
	mutable bool mAspectRatioChangedSinceLastRead = true;
#pragma warning( pop )
	friend class VertexRenderer;
};

