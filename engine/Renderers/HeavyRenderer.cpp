#include "HeavyRenderer.h"

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/MeshDataHeavy.h"
#include "../Helpers/Shader.h"

void HeavyRenderer::setup(const MeshDataHeavy* data, unsigned int vertexMode, unsigned int vertexLocation)
{
	mData = data;
	mIndicesMode = GL_TRIANGLES;
	mVertexMode = vertexMode;
	mVertexLocation = vertexLocation;
	validate();
	shader()->use();

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);
	if (!data->indices.empty())
	{
		glGenBuffers(1, &mEbo);
	}
	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	constexpr GLsizei vertexSize = sizeof(MeshDataHeavy::Vertex);
	constexpr GLsizei glmv3Size = glm::vec3::length();
	constexpr GLsizei glmv2Size = glm::vec2::length();

	glBufferData(GL_ARRAY_BUFFER, vertexSize * data->vertices.size(),
		data->vertices.data(), GL_STATIC_DRAW);

	if (data->indices.size())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			data->indices.size() * sizeof(unsigned int),
			data->indices.data(), GL_STATIC_DRAW);
	}

	// vertex Positions first
	GLint xx = shader()->getAttributeLocation("aPos");
	glEnableVertexAttribArray(xx);
	glVertexAttribPointer(xx, glmv3Size, GL_FLOAT, GL_FALSE, vertexSize, 0);

	xx = shader()->getAttributeLocation("aTexCoords");
	// vertex texture coords
	glEnableVertexAttribArray(xx);
	glVertexAttribPointer(xx, glmv2Size, GL_FLOAT, GL_FALSE, vertexSize,
		(GLvoid*)(glmv3Size * sizeof(float)));

	// vertex normals
	xx = shader()->getAttributeLocation("normal");
	if (xx != -1)
	{
		glEnableVertexAttribArray(xx);
		glVertexAttribPointer(xx, glmv3Size, GL_FLOAT, GL_FALSE, vertexSize,
			(GLvoid*)(sizeof(float) * (glmv3Size + glmv2Size)));
	}
	glBindVertexArray(0);
	if (data->indices.size())
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind the index buffer AFTER the vao has been unbound
}

void HeavyRenderer::doRender() const
{
	if (mData->textures.size())
	{
		// A nice explanation
		// https://community.khronos.org/t/what-is-a-texture-unit/63250
		//
		// https://www.reddit.com/r/opengl/comments/6gnc9x/trouble_with_framebuffer/
		for (const auto& tex : mData->textures)
		{
			glActiveTexture(tex.imageUnit());
			glBindTexture(tex.target(), tex.location());
			// associate sampler with textureImageUnit
			constShader()->setInteger(tex.samplerName(), tex.imageUnit() - GL_TEXTURE0);
		}
	}
	glBindVertexArray(mVao);

	if (mData->indices.size())
	{
		glDrawElements(mIndicesMode,
			static_cast<GLsizei>(mData->indices.size()), GL_UNSIGNED_INT, 0);
	}
	else if (mData->vertices.size())
	{
		glDrawArrays(mVertexMode, 0,
			static_cast<GLsizei>(mData->vertices.size()));
	}
	if (mData->textures.size())
	{
		// clean up.
		for (auto t : mData->textures)
		{
			glActiveTexture(t.imageUnit());
			glBindTexture(t.target(), 0);
		}
	}
	glBindVertexArray(0);
}

void HeavyRenderer::validate() const
{
	validateBase();
}
