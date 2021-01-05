#include "SimpleModelRenderer.h"

#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/ErrorHandling.h"
#include "../Helpers/CommonUtils.h"
#include "../Core/GlobalSettings.h"

#include "SimpleModel.h"

SimpleModelRenderer::SimpleModelRenderer()
{
}

SimpleModelRenderer::~SimpleModelRenderer()
{
}

void SimpleModelRenderer::prepare(const SimpleModel* source)
{
	checkSourceForErrors(source);
	OWUtils::PolygonModeRIAA poly;
	glGenVertexArrays(1, &mVao);
	// bind the Vertex Array Object first, then bind and set 
	// vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	source->shader()->use();
	source->shader()->setVector4f(source->mColourName, source->mColour);

	if (!source->mIndices.empty())
	{
		glGenBuffers(1, &mEbo);
	}

	if (source->mVec3.size())
	{
		glVertexAttribPointer(source->mVertexLoc,
			3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(source->mVertexLoc);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * source->mVec3.size(),
			source->mVec3.data(), GL_STATIC_DRAW);
	}
	else if (source->mVec4.size())
	{
		glVertexAttribPointer(source->mVertexLoc,
			4,
			GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(source->mVertexLoc);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * source->mVec4.size(),
			source->mVec4.data(), GL_STATIC_DRAW);
	}

	if (!source->mIndices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * static_cast<GLsizei>(source->mIndices.size()),
			source->mIndices.data(), GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer 
	// registered VBO as the vertex attribute's bound vertex buffer 
	// object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void SimpleModelRenderer::render(const SimpleModel* source,
				const glm::mat4& proj,
				const glm::mat4& view, const glm::mat4& model,
				OWUtils::RenderCallbackType renderCb,
				OWUtils::ResizeCallbackType resizeCb) const
{
	OWUtils::PolygonModeRIAA poly;
	source->shader()->use();
	setPVM(source, proj, view, model);
	if (!source->mColourName.empty())
	{
		source->shader()->setVector4f(source->mColourName, source->mColour);

	}
	glBindVertexArray(mVao);
	if (source->mTextures.size())
	{
		// A nice explanation
		// https://community.khronos.org/t/what-is-a-texture-unit/63250
		//
		// https://www.reddit.com/r/opengl/comments/6gnc9x/trouble_with_framebuffer/
		for (const auto& tex : source->mTextures)
		{
			glActiveTexture(tex.imageUnit());
			glBindTexture(tex.target(), tex.location());
			// associate sampler with name in shader
			source->shader()->setInteger(tex.samplerName(), tex.imageUnit() - GL_TEXTURE0);
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
//			glActiveTexture(t.imageUnit());
//			glBindTexture(t.target(), 0);
		}
	}
}

void SimpleModelRenderer::checkSourceForErrors(const SimpleModel* source) const
{
	checkBaseSourceForErrors(source);
	if (source->mRenderer == nullptr)
		throw NMSLogicException("source->mRenderer must not be null");
	if (source->mIndices.size() && source->mIndicesMode == GL_INVALID_ENUM)
		throw NMSLogicException("mIndicesMode is unset");
	//source->mTexture = 0;
}