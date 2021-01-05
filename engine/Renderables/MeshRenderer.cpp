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
#define USINGVERTEX

#ifdef USINGVERTEX
	checkSourceForErrors(source);
	source->shader()->use();

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo[0]);
	if (!source->indices().empty())
	{
		glGenBuffers(1, &mEbo);
	}
	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);

	const GLsizei vertexSize = sizeof(MeshData::Vertex);
	const GLsizei glmv3Size = glm::vec3::length();
	const GLsizei glmv2Size = glm::vec2::length();

	glBufferData(GL_ARRAY_BUFFER, vertexSize * source->vertices().size(),
		source->vertices().data(), GL_STATIC_DRAW);

	if (source->indices().size())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			source->indices().size() * sizeof(unsigned int),
			source->indices().data(), GL_STATIC_DRAW);
	}

	// vertex Positions first
	GLint xx = source->shader()->getAttributeLocation("aPos");
	glEnableVertexAttribArray(xx);
	glVertexAttribPointer(xx, glmv3Size, GL_FLOAT, GL_FALSE, vertexSize, 0);
	xx = source->shader()->getAttributeLocation("aTexCoords");
	// vertex texture coords
	glEnableVertexAttribArray(xx);
	glVertexAttribPointer(xx, glmv2Size, GL_FLOAT, GL_FALSE, vertexSize, 
			(GLvoid*)(glmv3Size * sizeof(float)));

#ifdef MESH_HAS_NORMAL
	// vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, glmv3Size, GL_FLOAT, GL_FALSE, vertexSize, 
			(GLvoid*)(sizeof(float) * (glmv3Size + glmv2Size)));
#endif
	glBindVertexArray(0);
#else
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	for (auto& v : source->vertices())
	{
		positions.push_back(v.position);
		texCoords.push_back(v.textureCoord);
	}
	checkSourceForErrors(source);
	source->shader()->use();
	glGenVertexArrays(1, &mVao);
	glGenBuffers(2, &mVbo[0]);
	if (!source->indices().empty())
	{
		glGenBuffers(1, &mEbo);
	}
	const GLsizei vertexSize = sizeof(MeshData::Vertex);
	const GLsizei glmv3Size = glm::vec3::length();
	const GLsizei glmv2Size = glm::vec2::length();

	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * glmv3Size * positions.size(),
		positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * glmv2Size * texCoords.size(),
		texCoords.data(), GL_STATIC_DRAW);

	if (source->indices().size())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			source->indices().size() * sizeof(unsigned int),
			source->indices().data(), GL_STATIC_DRAW);
	}

	// vertex Positions first
	GLint xx = source->shader()->getAttributeLocation("aPos");
	glEnableVertexAttribArray(xx);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
	glVertexAttribPointer(xx, glmv3Size, GL_FLOAT, GL_FALSE, 0, 0);

	xx = source->shader()->getAttributeLocation("aTexCoords");
	// vertex texture coords
	glEnableVertexAttribArray(xx);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
	glVertexAttribPointer(xx, glmv2Size, GL_FLOAT, GL_FALSE, 0, 0);

#ifdef MESH_HAS_NORMAL
	// vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, glmv3Size, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)(glmv3Size + glmv2Size));
#endif
	//	glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer 
		// registered VBO as the vertex attribute's bound vertex buffer 
		// object so afterwards we can safely unbind
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't 
		// accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't 
		// unbind VAOs (nor VBOs) when it's not directly necessary.
	//	glBindVertexArray(0);
#endif
}

void MeshRenderer::render(const Mesh* source,
	const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4& model,
	OWUtils::RenderCallbackType renderCb,
	OWUtils::ResizeCallbackType resizeCb) const
{
	OWUtils::PolygonModeRIAA poly;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
			glm::mat4 pv = proj * view;
			source->shader()->setMatrix4(source->pvmName(), pv);
		}
	}
	if (source->textures().size())
	{
		// A nice explanation
		// https://community.khronos.org/t/what-is-a-texture-unit/63250
		//
		// https://www.reddit.com/r/opengl/comments/6gnc9x/trouble_with_framebuffer/
		for (const auto& tex : source->textures())
		{
			glActiveTexture(tex.imageUnit());
			glBindTexture(tex.target(), tex.location());
			// associate sampler with textureImageUnit
			source->shader()->setInteger(tex.samplerName(), tex.imageUnit() - GL_TEXTURE0);
		}
	}
	glBindVertexArray(mVao);
	//callResizeCallback(source, resizeCb);
	//callRenderCallback(source, proj, view, model, renderCb);

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
	if (source->textures().size())
	{
		// clean up.
		for (auto t : source->textures())
		{
			glActiveTexture(t.imageUnit());
			glBindTexture(t.target(), 0);
		}
	}
	glBindVertexArray(0);
}

void MeshRenderer::checkSourceForErrors(const Mesh* source) const
{
	checkRenderBaseForErrors(source);
	if (source->mRenderer == nullptr)
		throw NMSLogicException("source->mRenderer must not be null");
	if (!source->mData.vertices.size())
		throw NMSLogicException("model has no data");
}