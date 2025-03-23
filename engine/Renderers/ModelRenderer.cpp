#include "ModelRenderer.h"

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/Shader.h"
void OWModelRenderer::add(const Texture& texture)
{
}

void OWModelRenderer::add(const OWModelData& modelData)
{
	mData = modelData;
}

void OWModelRenderer::doSetup(const OWRenderData& renderData)
{
	if (renderData.models.size() == 0)
		throw NMSLogicException(
			"Error. OWModelRenderer::doSetup has no models\n");
	if (renderData.models.size() > 1)
		throw NMSLogicException(
			"Error. OWModelRenderer::doSetup cannot process multiple models\n");
	if (renderData.textures.size() == 1)
	{
		add(renderData.textures[0]);
	}
	if (renderData.meshes.size() > 0)
	{
		throw NMSLogicException(
			"Error. OWModelRender cannot draw Meshes \n");
	}
	for (const auto& m : renderData.models)
	{
		add(m);
	}
	continueSetup();
}

void OWModelRenderer::continueSetup()
{
	shader()->use();
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	// Add one for the vertex array 
	glGenBuffers(mSSBO.populatedUnSplicedArrayCount() + 1, &mVbo[0]);
	if (mSSBO.dataExists(GPUBufferObject::BufferStyle::SSBO))
	{
		// This link is good 
		// https://www.khronos.org/opengl/wiki/Vertex_Shader/Defined_Inputs
		// https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
		// https://ktstephano.github.io/rendering/opengl/ssbos
		glCreateBuffers(1, &mSbo);
		size_t sf = sizeof(float);
		size_t sf1 = sizeof(void*);
		size_t sz = mSSBO.splicedData.size() * sizeof(float);
		glNamedBufferStorage(mSbo,
			sz,
			(const void*)mSSBO.splicedData.data(),
			GL_DYNAMIC_STORAGE_BIT);
		// Note the 1 matches our binding = 1 in the vertex shader
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mSbo);
	}
	else if (mSSBO.dataExists(GPUBufferObject::BufferStyle::Uniform))
	{
		for (int i = 0; i < mSSBO.unsplicedData.size(); i++)
		{
			unsigned int location = i + 1;
			glEnableVertexAttribArray(location);
			glBindBuffer(GL_ARRAY_BUFFER, mVbo[i + 1]);
			glVertexAttribPointer(
				location, // must match the layout in the shader.
				mSSBO.unsplicedData[i].span, // glm::vec4
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
			);
			glBufferData(GL_ARRAY_BUFFER, mSSBO.unsplicedData[i].span * sizeof(float) * mSSBO.instanceCount(),
				mSSBO.unsplicedData[i].data.data(), GL_STREAM_DRAW);
			glVertexAttribDivisor(location, 1);
		}
	}

	if (!mData.indices.empty())
	{
		glGenBuffers(1, &mEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		size_t sz = sizeof(unsigned int) * static_cast<GLsizei>(mData.indices.size());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz, mData.indices.data(), GL_STATIC_DRAW);
	}

	if (mData.vertices.size())
	{
		constexpr GLsizei vertexSize = sizeof(OWModelData::Vertex);
		constexpr GLsizei glmv3Size = glm::vec3::length();
		constexpr GLsizei glmv2Size = glm::vec2::length();
		glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);

		// vertex Positions first
		GLint location = shader()->getAttributeLocation("in_vertex");
		glEnableVertexAttribArray(location);
		glBufferData(GL_ARRAY_BUFFER, vertexSize * mData.vertices.size(),
			mData.vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(location, glmv3Size, GL_FLOAT, GL_FALSE, vertexSize, 0);

		location = shader()->getAttributeLocation("aTexCoords");
		if (location != -1)
		{
			// vertex texture coords
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, glmv2Size, GL_FLOAT, GL_FALSE, vertexSize,
				(GLvoid*)(glmv3Size * sizeof(float)));
		}
		location = shader()->getAttributeLocation("normal");
		if (location != -1)
		{
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, glmv3Size, GL_FLOAT, GL_FALSE, vertexSize,
				(GLvoid*)(sizeof(float) * (glmv3Size + glmv2Size)));
		}
	}

	// vertex normals
	glBindVertexArray(0);
	if (mData.indices.size())
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind the index buffer AFTER the vao has been unbound
}

void OWModelRenderer::doRender() 
{
	glBindVertexArray(mVao);
	if (mSSBO.dataExists(GPUBufferObject::BufferStyle::SSBO))
	{
		// Does not appear to impact performance at all
		// Note the 1 matches our binding = 1 in the vertex shader
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mSbo);
	}

	if (mData.textures.size())
	{
		// A nice explanation
		// https://community.khronos.org/t/what-is-a-texture-unit/63250
		//
		// https://www.reddit.com/r/opengl/comments/6gnc9x/trouble_with_framebuffer/
		for (const auto& tex : mData.textures)
		{
			glActiveTexture(tex.imageUnit());
			glBindTexture(tex.target(), tex.location());
			// associate sampler with textureImageUnit
			shader()->setInteger(tex.samplerName(), tex.imageUnit() - GL_TEXTURE0);
		}
	}

	if (mData.indices.size())
	{
		unsigned int im = indicesMode();
		glDrawElements(indicesMode(),
			static_cast<GLsizei>(mData.indices.size()), GL_UNSIGNED_INT, 0);
	}
	else if (mData.vertices.size())
	{
		glDrawArrays(vertexMode(), 0,
			static_cast<GLsizei>(mData.vertices.size()));
	}
	if (mData.textures.size())
	{
		// clean up.
		for (auto t : mData.textures)
		{
			glActiveTexture(t.imageUnit());
			glBindTexture(t.target(), 0);
		}
	}
	glBindVertexArray(0);
}
