#include "RendererBase.h"

#include "../Core/ErrorHandling.h"
#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"

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
			glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			static bool onceOnly = true;
			if (onceOnly)
			{
				mSfactor = GL_SRC_ALPHA; // jfw not sure why I did this? maybe delete both statics
				mDfactor = GL_ONE_MINUS_SRC_ALPHA;
				onceOnly = false;
			}
			glBlendFunc(sfactor, dfactor);
		}
	}
	~BlendFuncRIAA()
	{
		if (mActive)
		{
			glDisable(GL_BLEND);
			glBlendFunc(mSfactor, mDfactor);
		}
	}
};

GLenum BlendFuncRIAA::mSfactor;
GLenum BlendFuncRIAA::mDfactor;

void OWRenderer::validateBase() const
{
	if (mShader == nullptr)
		throw NMSLogicException("RendererBase::Shader must be set");
}

void OWRenderer::render(std::vector<glm::mat4> models, const glm::mat4& proj,
	const glm::mat4& view, const glm::vec3& cameraPos)
{
	PolygonModeRIAA temp1(mPolygonFace, mPolygonMode);
	LineWidthRIAA temp2(mLineWidth);
	BlendFuncRIAA temp3(mSfactor, mDfactor);

	shader()->use();
	shader()->setStandardUniformValues(proj, view, models[0], cameraPos);
	shader()->callMutators(proj, view, models[0], cameraPos, nullptr);

	doRender();
}
