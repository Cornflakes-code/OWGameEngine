#include "RenderBase.h"

#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"
#include "../Helpers/ErrorHandling.h"

#include "RenderableBase.h"

RenderBase::RenderBase()
{}

float RenderBase::aspectRatio() const
{
	return globals->physicalWindowSize().x / (globals->physicalWindowSize().y * 1.0f);
}

glm::vec2 RenderBase::scaleByAspectRatio(const glm::vec2& toScale) const
{
	// This seems to work best (trial and error) when resizing the window.
	glm::vec2 retval = toScale;
	float _aspectRatio = aspectRatio();
	if (_aspectRatio < 1)
	{
		retval.x /= _aspectRatio;
		retval.y *= _aspectRatio;
	}
	else
	{
		retval.x /= _aspectRatio;
		//retval.y *= _aspectRatio ;
	}
	return retval;
}

void RenderBase::checkRenderBaseForErrors(const RenderableBase* source)
{
	if (source->mShader == nullptr)
		throw NMSLogicException("source->mShader must not be null");
	if (source->mPVMName.empty())
		throw NMSLogicException("mPVMName is empty");
}

void RenderBase::callResizeCallback(const RenderableBase* source,
	OWUtils::ResizeCallbackType resizeCb) const
{
	if (mFirstTimeRender || globals->aspectRatioChanged())
	{
		// If no callback parameters then used the stored callbacks
		if (!resizeCb)
			resizeCb = source->mResizeCallback;
		if (resizeCb)
		{
			resizeCb(source->mShader,
				std::bind(&RenderBase::scaleByAspectRatio,
					this, std::placeholders::_1),
				aspectRatio());
			mFirstTimeRender = false;
		}
	}
}

void RenderBase::callRenderCallback(const RenderableBase* source,
	const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	OWUtils::RenderCallbackType renderCb) const
{
	// Allow callers to override the callback stored in source
	if (!renderCb)
		renderCb = source->mRenderCallback;
	if (renderCb)
	{
		renderCb(proj, view, model, source->mShader);
	}
}

