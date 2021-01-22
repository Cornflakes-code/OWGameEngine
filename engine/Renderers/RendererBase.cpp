#include "RendererBase.h"

#include "../Core/ErrorHandling.h"
#include "../Core/GlobalSettings.h"

#include "../Helpers/Shader.h"


void RendererBase::validateBase() const
{
	if (mShader == nullptr)
		throw NMSLogicException("RendererBase::mShader must be set");
	if (mPvm.empty())
		throw NMSLogicException("pvm shader variable must have a name");
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

void RendererBase::setPVM(const glm::mat4& proj,
						const glm::mat4& view,
						const glm::mat4& model) const
{
	if (!mPvm.empty())
	{
		// Testing for name.size() is seriously tacky.
		// Must be a better way !!
		if (mPvm.size() < 3)
		{
			glm::mat4 pvm = proj * view;
			mShader->setMatrix4(mPvm, pvm);
		}
		else
		{
			glm::mat4 pvm = proj * view * model;
			mShader->setMatrix4(mPvm, pvm);
		}
	}
}

void RendererBase::callResizeCallback(ResizeCallbackType resizeCb) const
{
	if (mFirstTimeRender || globals->aspectRatioChanged())
	{
		// If no callback parameters then used the stored callbacks
		shader()->use();
		if (resizeCb)
		{
			resizeCb(shader(),
				std::bind(&RendererBase::scaleByAspectRatio, this, std::placeholders::_1),
				aspectRatio());
		}
		for (auto& cb: mResizeCallbacks)
		{
			cb(shader(), std::bind(&RendererBase::scaleByAspectRatio,
				this, std::placeholders::_1),
				aspectRatio());
			mFirstTimeRender = false;
		}
	}
}

void RendererBase::callRenderCallback(glm::mat4& proj, glm::mat4& view, 
	glm::mat4& model, RenderCallbackType renderCb) const
{
	// Allow callers and derived classes to override the callback
	shader()->use();
	if (renderCb)
	{
		renderCb(proj, view, model, shader());
	}
	for (auto& cb : mRenderCallbacks)
	{
		cb(proj, view, model, shader());
	}
}

void RendererBase::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4& model,
	const MoveController* mover, RenderCallbackType renderCb,
	ResizeCallbackType resizeCb) const
{
	OWUtils::PolygonModeRIAA temp1(mPolygonFace, mPolygonMode);
	OWUtils::LineWidthRIAA temp2(mLineWidth);
	shader()->use();
	callResizeCallback(resizeCb);
	glm::mat4 p = proj;
	glm::mat4 v = view;
	glm::mat4 m = model;
	callRenderCallback(p, v, m, renderCb);
	setPVM(p, v, m);

	doRender();
}
