#include "MeshRenderer.h"

#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/ErrorHandling.h"
#include "../Helpers/CommonUtils.h"
#include "../Core/GlobalSettings.h"

#include "Mesh.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::prepare(const Mesh* source)
{
	checkSourceForErrors(source);
	OWUtils::PolygonModeRIAA poly;
	source->shader()->use();
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);
	if (source->indices().size())
	{
		glGenBuffers(1, &mEbo);
	}
	glBindVertexArray(mVao);

	GLsizei vertexSize = sizeof(MeshData::Vertex);

	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, vertexSize * source->vertices().size(),
		source->vertices().data(), GL_STATIC_DRAW);

	if (source->indices().size())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
				source->indices().size() * sizeof(unsigned int), 
				source->indices().data(), GL_STATIC_DRAW);
	}
	GLsizei glmv3Size = glm::vec3::length();
	GLsizei glmv2Size = glm::vec2::length();

	// vertex Positions first
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, // attribute. must match the layout in the shader.
		glmv3Size, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		vertexSize, // stride
		0); // array buffer offset
	glEnableVertexAttribArray(1);
	// vertex normals
	glVertexAttribPointer(1, // attribute. must match the layout in the shader.
		glmv3Size, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		vertexSize, // stride
		(GLvoid*)glmv3Size);

	glEnableVertexAttribArray(2);
	// vertex texture coords
	glVertexAttribPointer(2, // attribute. must match the layout in the shader.
		glmv2Size, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		vertexSize, // stride
		(GLvoid*)glmv3Size);

	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void MeshRenderer::render(const Mesh* source,
	const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4& model,
	OWUtils::RenderCallbackType renderCb,
	OWUtils::ResizeCallbackType resizeCb) const
{
	OWUtils::PolygonModeRIAA poly;
	source->shader()->use();
	if (!source->pvmName().empty())
	{
		// Testing for name.size() is seriously tacky.
		// Must be a better way !!
		if (source->includeModelInPVM())
		{
			glm::mat4 pvm = proj * view * model;
			source->shader()->setMatrix4(source->pvmName(), pvm);
		}
		else
		{
			glm::mat4 pvm = proj * view;
			source->shader()->setMatrix4(source->pvmName(), pvm);
		}
	}
	glBindVertexArray(mVao);
	if (source->textures().size())
	{
		// A nice explanation
		// https://community.khronos.org/t/what-is-a-texture-unit/63250
		//
		// https://www.reddit.com/r/opengl/comments/6gnc9x/trouble_with_framebuffer/
		// This should be obtained from Mesh
		// bind mTextureLoc to a texture image unit (usually GL_TEXTURE0).
		for (const auto& tex : source->textures())
		{
			glActiveTexture(tex.imageUnit());
			glBindTexture(tex.target(), tex.location());
			// associate sampler with textureImageUnit
			source->shader()->setInteger("textureImageId",
				tex.imageUnit() - GL_TEXTURE0);
		}
	}
	callResizeCallback(source, resizeCb);
	callRenderCallback(source, proj, view, model, renderCb);

	GLsizei sz = static_cast<GLsizei>(source->indices().size());
	if (sz)
	{
		glDrawElements(source->indiceMode(),
			static_cast<GLsizei>(sz), GL_UNSIGNED_INT, 0);
	}
	else if (source->vertices().size())
	{
		glDrawArrays(source->vertexMode(), 0,
			static_cast<GLsizei>(source->vertices().size()));
	}
	glBindVertexArray(0);
	if (source->textures().size())
	{
		// clean up.
		for (auto t : source->textures())
		{
			glActiveTexture(t.imageUnit());
			glBindTexture(t.target(), 0);
		}
	}

}

void MeshRenderer::checkSourceForErrors(const Mesh* source) const
{
	checkRenderBaseForErrors(source);
	if (source->mRenderer == nullptr)
		throw NMSLogicException("source->mRenderer must not be null");
	if (!source->mData.vertices.size())
		throw NMSLogicException("model has no data");
}