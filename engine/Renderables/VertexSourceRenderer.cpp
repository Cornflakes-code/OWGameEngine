#include "VertexSourceRenderer.h"

#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/ResourceFactory.h"
#include "../Helpers/ErrorHandling.h"
#include "../Helpers/CommonUtils.h"
#include "../Core/GlobalSettings.h"

#include "VertexSource.h"

VertexSourceRenderer::VertexSourceRenderer()
{
}

VertexSourceRenderer::~VertexSourceRenderer()
{
}

void VertexSourceRenderer::prepare(const VertexSource* source)
{
	checkSourceForErrors(source);
	OWUtils::PolygonModeRIAA poly;
	glGenVertexArrays(1, &mVao);
	// bind the Vertex Array Object first, then bind and set 
	// vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	source->mShader->use();
	source->mShader->setVector4f(source->mColourName, source->mColour);

	if (!source->mIndices.empty())
	{
		glGenBuffers(1, &mEbo);
	}
	checkGLError();

	if (source->mVec3.size())
	{
		glVertexAttribPointer(source->mVertexLoc,
			3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(source->mVertexLoc);
		checkGLError();
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * source->mVec3.size(),
			source->mVec3.data(), GL_STATIC_DRAW);
		checkGLError();
	}
	else if (source->mVec4.size())
	{
		glVertexAttribPointer(source->mVertexLoc,
			4,
			GL_FLOAT, GL_FALSE, 0, (void*)0);
		checkGLError();

		glEnableVertexAttribArray(source->mVertexLoc);
		checkGLError();
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * source->mVec4.size(),
			source->mVec4.data(), GL_STATIC_DRAW);
		checkGLError();
	}

	if (!source->mIndices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		checkGLError();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * static_cast<GLsizei>(source->mIndices.size()),
			source->mIndices.data(), GL_STATIC_DRAW);
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

void VertexSourceRenderer::render(const VertexSource* source,
				const glm::mat4& proj,
				const glm::mat4& view, const glm::mat4& model,
				OWUtils::RenderCallbackType renderCb,
				OWUtils::ResizeCallbackType resizeCb) const
{
	OWUtils::PolygonModeRIAA poly;
	source->mShader->use();
	renderPVM(source, proj, view, model);
	if (!source->mColourName.empty())
	{
		source->mShader->setVector4f(source->mColourName, source->mColour);

	}
	glBindVertexArray(mVao);
	if (source->mTextures.size())
	{
		// A nice explanation
		// https://community.khronos.org/t/what-is-a-texture-unit/63250
		//
		// https://www.reddit.com/r/opengl/comments/6gnc9x/trouble_with_framebuffer/
		// This should be obtained from VertexSource
		// bind mTextureLoc to a texture image unit (usually GL_TEXTURE0).
		for (auto t : source->mTextures)
		{
			glActiveTexture(t.imageUnit);
			glBindTexture(t.target, t.location);
			// associate sampler with textureImageUnit
			source->mShader->setInteger(t.name, t.imageUnit - GL_TEXTURE0);
		}
	}
	callResizeCallback(source, resizeCb);
	callRenderCallback(source, proj, view, model, renderCb);

	GLsizei sz = static_cast<GLsizei>(source->mIndices.size());
	if (source->mIndices.size())
	{
		glDrawElements(source->mIndicesMode,
			static_cast<GLsizei>(sz), GL_UNSIGNED_INT, 0);
	}
	else if (source->mVec3.size())
	{
		glDrawArrays(source->mVertexMode, 0,
			static_cast<GLsizei>(source->mVec3.size()));
	}
	else if (source->mVec4.size())
	{
		glDrawArrays(source->mVertexMode, 0,
			static_cast<GLsizei>(source->mVec4.size()));
	}
	glBindVertexArray(0);
	if (source->mTextures.size())
	{
		// clean up.
		for (auto t : source->mTextures)
		{
			glActiveTexture(t.imageUnit);
			glBindTexture(t.target, 0);
		}
	}
}

void VertexSourceRenderer::checkSourceForErrors(const VertexSource* source)
{
	checkBaseSourceForErrors(source);
	if (source->mRenderer == nullptr)
		throw NMSLogicException("source->mRenderer must not be null");
	if (source->mIndices.size() && source->mIndicesMode == GL_INVALID_ENUM)
		throw NMSLogicException("mIndicesMode is unset");
	//source->mTexture = 0;
}