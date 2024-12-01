#include "RendererBase.h"

#include "../Core/ErrorHandling.h"
#include "../Core/GlobalSettings.h"

#include "../Helpers/Shader.h"


void RendererBase::validateBase() const
{
	if (constShader() == nullptr)
		throw NMSLogicException("RendererBase::Shader must be set");
}

float RendererBase::aspectRatio() const
{
	return globals->physicalWindowSize().x / 
		(globals->physicalWindowSize().y * 1.0f);

}

glm::vec2 RendererBase::scaleByAspectRatio(const glm::vec2& toScale) const
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

void RendererBase::callResizeCallback(ResizeCallbackType resizeCb) const
{
	if (mFirstTimeRender || globals->aspectRatioChanged())
	{
		// If no callback parameters then used the stored callbacks
		constShader()->use();
		if (resizeCb)
		{
			resizeCb(constShader(),
				std::bind(&RendererBase::scaleByAspectRatio, this, std::placeholders::_1),
				aspectRatio());
		}
		for (auto& cb: mResizeCallbacks)
		{
			cb(constShader(), std::bind(&RendererBase::scaleByAspectRatio,
				this, std::placeholders::_1),
				aspectRatio());
			mFirstTimeRender = false;
		}
	}
}

void RendererBase::callRenderCallback(const glm::mat4& proj, const glm::mat4& view,
	const glm::mat4& model, const glm::vec3& cameraPos, RenderCallbackType renderCb) const
{
	// Allow callers and derived classes to override the callback
	constShader()->use();
	if (renderCb)
	{
		renderCb(proj, view, model, cameraPos, constShader());
	}
	for (auto& cb : mRenderCallbacks)
	{
		cb(proj, view, model, cameraPos, constShader());
	}
}

void RendererBase::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4& model,
	const glm::vec3& cameraPos,
	MoveController* mover, 
	RenderCallbackType renderCb,
	ResizeCallbackType resizeCb) const
{
	OWUtils::PolygonModeRIAA temp1(mPolygonFace, mPolygonMode);
	OWUtils::LineWidthRIAA temp2(mLineWidth);
	OWUtils::BlendFuncRIAA temp3(mSfactor, mDfactor);

	constShader()->use();
	callResizeCallback(resizeCb);
	const_cast<Shader*>(constShader())->setStandardUniformValues(proj, view, model, cameraPos);
	callRenderCallback(proj, view, model, cameraPos, renderCb);

	doRender();
}
