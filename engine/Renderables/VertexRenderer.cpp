#include "VertexRenderer.h"

#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/ResourceFactory.h"
#include "../Helpers/ErrorHandling.h"
#include "../Helpers/CommonUtils.h"
#include "../Core/GlobalSettings.h"

VertexRenderer::VertexRenderer()
{
}

VertexRenderer::~VertexRenderer()
{
}

float VertexRenderer::aspectRatio() const
{
	return globals->physicalWindowSize().x / (globals->physicalWindowSize().y * 1.0f);
}

glm::vec2 VertexRenderer::scaleByAspectRatio(const glm::vec2& toScale) const
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

void VertexRenderer::prepareOpenGL()
{
	OWUtils::PolygonModeRIAA poly;
	glGenVertexArrays(1, &mVao);
	// bind the Vertex Array Object first, then bind and set 
	// vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	mSource->mShader->use();
	mSource->mShader->setVector4f(mSource->mColourName, mSource->mColour);

	if (!mSource->mIndices.empty())
	{
		glGenBuffers(1, &mEbo);
	}
	checkGLError();

	if (mSource->mVec3.size())
	{
		glVertexAttribPointer(mSource->mVertexLocation,
			3,
			GL_FLOAT, GL_FALSE, 0, (void*)0);
		checkGLError();
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mSource->mVec3.size(),
			mSource->mVec3.data(), GL_STATIC_DRAW);
		checkGLError();
	}
	else if (mSource->mVec4.size())
	{
		glVertexAttribPointer(mSource->mVertexLocation,
			4,
			GL_FLOAT, GL_FALSE, 0, (void*)0);
		checkGLError();

		glEnableVertexAttribArray(mSource->mVertexLocation);
		checkGLError();
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mSource->mVec4.size(),
			mSource->mVec4.data(), GL_STATIC_DRAW);
		checkGLError();
	}

	if (!mSource->mIndices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		checkGLError();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * static_cast<GLsizei>(mSource->mIndices.size()),
			mSource->mIndices.data(), GL_STATIC_DRAW);
		checkGLError();
	}

	glEnableVertexAttribArray(0);
	checkGLError();

	// note that this is allowed, the call to glVertexAttribPointer 
	// registered VBO as the vertex attribute's bound vertex buffer 
	// object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkGLError();

	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	checkGLError();
}

void VertexRenderer::render( const glm::mat4& proj,
				const glm::mat4& view, const glm::mat4& model,
				VertexSource::RenderCallbackType renderCb,
				VertexSource::ResizeCallbackType resizeCb) const
{
	mSource->mShader->use();
	if (!mSource->mPVMName.empty())
	{
		// Testing for name.size() is seriously tacky.
		// Must be a better way !!
		if (mSource->mPVMName.size() < 3)
		{
			glm::mat4 pvm = proj * view;
			mSource->mShader->setMatrix4(mSource->mPVMName, pvm);
		}
		else
		{
			glm::mat4 pvm = proj * view * model;
			mSource->mShader->setMatrix4(mSource->mPVMName, pvm);
		}
	}
	if (!mSource->mColourName.empty())
	{
		mSource->mShader->setVector4f(mSource->mColourName, mSource->mColour);

	}
	glBindVertexArray(mVao);
	if (mSource->mTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mSource->mTexture);
	}

	if (globals->aspectRatioChanged())
	{
		// If no callback parameters then used the stored callbacks
		if (!resizeCb)
			resizeCb = mSource->mResizeCallback;
		if (resizeCb)
		{
			resizeCb(mSource->mShader, 
				std::bind(&VertexRenderer::scaleByAspectRatio,
						this, std::placeholders::_1),
				aspectRatio());
		}
	}

	// Allow callers to override the callback stored in mSource
	if (!renderCb)
		renderCb = mSource->mRenderCallback;
	if (renderCb)
	{
		renderCb(proj, view, model, mSource->mShader);
	}

	GLsizei sz = static_cast<GLsizei>(mSource->mIndices.size());
	if (mSource->mIndices.size())
	{
		glDrawElements(mSource->mIndicesMode,
			static_cast<GLsizei>(sz), GL_UNSIGNED_INT, 0);
	}
	else if (mSource->mVec3.size())
	{
		glDrawArrays(mSource->mVertexMode, 0,
			static_cast<GLsizei>(mSource->mVec3.size()));
	}
	else if (mSource->mVec4.size())
	{
		glDrawArrays(mSource->mVertexMode, 0,
			static_cast<GLsizei>(mSource->mVec4.size()));
	}
	glBindVertexArray(0);
	if (mSource->mTexture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void VertexRenderer::checkSourceForErrors()
{
	if (mSource->mShader == nullptr)
		throw NMSLogicException("mSource->mShader should not be null");
	if (mSource->mVec3.size() == 0 && mSource->mVec4.size() == 0)
		throw NMSLogicException("both mVec3 and mVec4 are empty");
	if (mSource->mVec3.size() != 0 && mSource->mVec4.size() != 0)
		throw NMSLogicException("both mVec3 and mVec4 are not empty");
	if (mSource->mPVMName.empty())
		throw NMSLogicException("mPVMName is empty");
	if (mSource->mVertexLocation == GL_INVALID_INDEX)
		throw NMSLogicException("mVertexLocation is unset");
	if (mSource->mVertexMode == GL_INVALID_ENUM)
		throw NMSLogicException("mVertexMode is unset");
	if (mSource->mIndices.size() && mSource->mIndicesMode == GL_INVALID_ENUM)
		throw NMSLogicException("mIndicesMode is unset");
	//mSource->mTexture = 0;
}