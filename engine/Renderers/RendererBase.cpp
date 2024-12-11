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
/*
* Following three classes use the dtor to restore values after being called in render.
*/
class OWENGINE_API PolygonModeRIAA
{
	static GLint mOriginalMode;
	bool mActive = true;
public:
	PolygonModeRIAA(GLenum face, GLenum mode)
		: mActive(face != UINT_MAX && mode != UINT_MAX)
	{
		if (mActive)
		{
			static bool onceOnly = true;
			if (onceOnly)
			{
				glGetIntegerv(GL_POLYGON_MODE, &mOriginalMode);
				onceOnly = false;
			}
			glPolygonMode(face, mode);
		}
	}
	~PolygonModeRIAA()
	{
		if (mActive)
			glPolygonMode(GL_FRONT_AND_BACK, mOriginalMode);
	}
};
GLint PolygonModeRIAA::mOriginalMode;

class OWENGINE_API LineWidthRIAA
{
	static GLfloat mOriginalWidth;
	bool mActive = true;
public:
	LineWidthRIAA(float width)
		: mActive(width >= 0)
	{
		if (mActive)
		{
			static bool onceOnly = true;
			if (onceOnly)
			{
				glGetFloatv(GL_LINE_WIDTH, &mOriginalWidth);
				onceOnly = false;
			}
			glLineWidth(width);
		}
	}
	~LineWidthRIAA()
	{
		if (mActive)
			glLineWidth(mOriginalWidth);
	}
};
GLfloat LineWidthRIAA::mOriginalWidth;

class OWENGINE_API BlendFuncRIAA
{
	static GLenum mSfactor;
	static GLenum mDfactor;
	bool mActive = true;
public:
	BlendFuncRIAA(GLenum sfactor, GLenum dfactor)
		: mActive(sfactor != UINT_MAX && dfactor != UINT_MAX)
	{
		if (mActive)
		{
			static bool onceOnly = true;
			if (onceOnly)
			{
				mSfactor = GL_SRC_ALPHA;
				mDfactor = GL_ONE_MINUS_SRC_ALPHA;
				onceOnly = false;
			}
			glBlendFunc(sfactor, dfactor);
		}
	}
	~BlendFuncRIAA()
	{
		if (mActive)
			glBlendFunc(mSfactor, mDfactor);
	}
};

GLenum BlendFuncRIAA::mSfactor;
GLenum BlendFuncRIAA::mDfactor;

void RendererBase::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4& model,
	const glm::vec3& cameraPos,
	RenderCallbackType renderCb,
	ResizeCallbackType resizeCb) const
{
	PolygonModeRIAA temp1(mPolygonFace, mPolygonMode);
	LineWidthRIAA temp2(mLineWidth);
	BlendFuncRIAA temp3(mSfactor, mDfactor);

	constShader()->use();
	callResizeCallback(resizeCb);
	const_cast<Shader*>(constShader())->setStandardUniformValues(proj, view, model, cameraPos);
	callRenderCallback(proj, view, model, cameraPos, renderCb);

	doRender();
}
