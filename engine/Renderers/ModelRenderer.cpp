#include "ModelRenderer.h"

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/Shader.h"
void OWModelRenderer::add(const Texture& texture)
{
}

void OWModelRenderer::add(const OWModelData& md)
{
	if (!md.vertices.empty())
	{
		mMultiArrayStartIndexes.push_back(static_cast<GLsizei>(mData.vertices.size()));
		mData.vertices.insert(mData.vertices.end(), md.vertices.begin(), md.vertices.end());
		mMultiArrayVertexCount.push_back(static_cast<GLsizei>(md.vertices.size()));
	}
	if (!md.indices.empty())
	{
		// https://stackoverflow.com/questions/24516993/is-it-possible-to-use-index-buffer-objects-ibo-with-the-function-glmultidrawe
		size_t xx = mData.indices.size() * sizeof(GLsizei);
		mMultiElementStartIndexes.push_back(reinterpret_cast<void*>(xx));
		mData.indices.insert(mData.indices.end(), md.indices.begin(), md.indices.end());
		mMultiElementIndicesCounts.push_back(static_cast<GLsizei>(mData.indices.size() * sizeof(unsigned int)));
	}
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
		size_t sz = mSSBO.splicedData.size() * sizeof(float);
		glNamedBufferStorage(mSbo,
			sz,
			(const void*)mSSBO.splicedData.data(),
			GL_MAP_WRITE_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mSSBO.shaderBinding(), mSbo);
		void* buf = glMapNamedBuffer(mSbo, GL_WRITE_ONLY);
		if (buf == nullptr)
		{
			throw NMSException("OWMeshRenderer.glMapNamedBuffer returned null. Unknown error.\n");
		}
		mSSBO.setWriteBuffer(buf);
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
	// https://www.jointaro.com/interview-insights/apple/describe-the-opengl-pipeline-stages-and-shader-roles/
	glBindVertexArray(mVao);
	if (mSSBO.dataExists(GPUBufferObject::BufferStyle::SSBO))
	{
		// Does not appear to impact performance at all
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mSSBO.shaderBinding(), mSbo);
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
		// https://www.khronos.org/opengl/wiki/Buffer_Object
		// Look at:
		// https://www.reddit.com/r/opengl/comments/19bgtcb/is_the_effort_of_glmultidrawelements_worth_it/
		// If says that glMultiDrawElements is obsolete and instead use glMultiDrawElementsIndirect

		glMultiDrawElements(vertexMode(), mMultiElementIndicesCounts.data(), GL_UNSIGNED_INT,
			(const void**)mMultiElementStartIndexes.data(), static_cast<GLsizei>(mMultiArrayStartIndexes.size()));
	}
	else if (mData.vertices.size())
	{
		glMultiDrawArrays(vertexMode(), mMultiArrayStartIndexes.data(),
			mMultiArrayVertexCount.data(), static_cast<GLsizei>(mMultiArrayStartIndexes.size()));
	}
	glBindVertexArray(0);
}
