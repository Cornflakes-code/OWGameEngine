#include "VertexRenderBase.h"

#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"
#include "../Helpers/ErrorHandling.h"

#include "Vertices.h"

VertexRenderBase::VertexRenderBase()
{}

float VertexRenderBase::aspectRatio() const
{
	return globals->physicalWindowSize().x / (globals->physicalWindowSize().y * 1.0f);
}

glm::vec2 VertexRenderBase::scaleByAspectRatio(const glm::vec2& toScale) const
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

void VertexRenderBase::checkBaseSourceForErrors(const Vertices* source)
{
	if (source->mShader == nullptr)
		throw NMSLogicException("source->mShader must not be null");
	if (source->mVec3.size() == 0 && source->mVec4.size() == 0)
		throw NMSLogicException("both mVec3 and mVec4 are empty");
	if (source->mVec3.size() != 0 && source->mVec4.size() != 0)
		throw NMSLogicException("both mVec3 and mVec4 are not empty");
	if (source->mPVMName.empty())
		throw NMSLogicException("mPVMName is empty");
	if (source->mVertexLoc == GL_INVALID_INDEX)
		throw NMSLogicException("mVertexLoc is unset");
	if (source->mVertexMode == GL_INVALID_ENUM)
		throw NMSLogicException("mVertexMode is unset");
}

void VertexRenderBase::callResizeCallback(const Vertices* source, 
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
				std::bind(&VertexRenderBase::scaleByAspectRatio,
					this, std::placeholders::_1),
				aspectRatio());
			mFirstTimeRender = false;
		}
	}
}

void VertexRenderBase::callRenderCallback(const Vertices* source,
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

void VertexRenderBase::renderPVM(const Vertices* source,
					const glm::mat4& proj,
					const glm::mat4& view,
					const glm::mat4& model) const
{
	if (!source->mPVMName.empty())
	{
		// Testing for name.size() is seriously tacky.
		// Must be a better way !!
		if (source->mPVMName.size() < 3)
		{
			glm::mat4 pvm = proj * view;
			source->mShader->setMatrix4(source->mPVMName, pvm);
		}
		else
		{
			glm::mat4 pvm = proj * view * model;
			source->mShader->setMatrix4(source->mPVMName, pvm);
		}
	}
}
