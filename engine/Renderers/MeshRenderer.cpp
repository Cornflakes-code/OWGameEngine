#include "MeshRenderer.h"


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/Shader.h"

void OWMeshRenderer::doSetup(const OWRenderData& renderData)
{
	if (renderData.models.size())
		throw NMSLogicException(
			"Error. OWMeshRender cannot draw OWModelData \n");
	if (renderData.textures.size() > 1)
	{
		throw NMSLogicException(
			"Error. OWMeshRender cannot handle more than one texture.\n");
	}

	if (renderData.textures.size() == 1)
	{
		add(renderData.textures[0]);
	}
	if (renderData.meshes.size() > 0)
	{
		mData.vertexMode = renderData.meshes[0].vertexMode;
		mData.indicesMode = renderData.meshes[0].indicesMode;
		mData.polygonMode_mode = renderData.meshes[0].polygonMode_mode;
#ifdef _DEBUG
		for (int i = 1; i < renderData.meshes.size(); i++)
		{
			if (renderData.meshes[i].vertexMode != renderData.meshes[i - 1].vertexMode)
				throw NMSLogicException(
					"OWMeshRenderer::doSetup Error. All meshes must have the same vertex mode. \n");
			if (renderData.meshes[i].indicesMode != renderData.meshes[i - 1].indicesMode)
				throw NMSLogicException(
					"OWMeshRenderer::doSetup Error. All meshes must have the same indices mode. \n");
			if (renderData.meshes[i].polygonMode_mode != renderData.meshes[i - 1].polygonMode_mode)
				throw NMSLogicException(
					"OWMeshRenderer::doSetup Error. All meshes must have the same polygon mode. \n");
		}
#endif
		for (const auto& m : renderData.meshes)
		{
			add(m);
		}
	}

	// https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
	if (mData.v4.size() == 0)
	{
		return;
	}
	if (mData.polygonMode_mode == UINT_MAX)
	{
		// mMeshData.polygonMode_mode = GL_FILL;
		throw NMSLogicException("OWMeshRenderer::doSetup(). Invalid polygonMode_mode.\n");
	}

	if (mData.polygonMode_mode == GL_LINES)
	{
		// Check for common error
		mData.polygonMode_mode = GL_LINE;
	}

	if (mVao != std::numeric_limits<unsigned int>::max())
		throw NMSLogicException("OWMeshRenderer::doSetup(). VAO should not be initialised.\n");
	continueSetup();
}

void OWMeshRenderer::continueSetup()
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
			GL_DYNAMIC_STORAGE_BIT);
		// Note the 1 matches our binding = 1 in the vertex shader
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mSbo);
	}

	if (!mData.indices.empty())
	{
		glGenBuffers(1, &mEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		size_t sz = sizeof(unsigned int) * static_cast<GLsizei>(mData.indices.size());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz, mData.indices.data(), GL_STATIC_DRAW);
	}
	unsigned int vertexLocation = 0;// location == 0 is always the mesh vertex
	if (mData.v4.size())
	{
		// Very nice explanation of glVertexAttribPointer and glEnableVertexAttribArray 
		// and how they tie into the shader
		// https://ktstephano.github.io/rendering/opengl/prog_vtx_pulling
		glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
		glVertexAttribPointer(vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(vertexLocation);
		size_t sz = mData.v4.size() * 4 * sizeof(float);
		glBufferData(GL_ARRAY_BUFFER, sz, mData.v4.data(), GL_STATIC_DRAW);
		glVertexAttribDivisor(vertexLocation, 0);
	}

	if (mSSBO.dataExists(GPUBufferObject::BufferStyle::Uniform))
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
	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (mData.indices.size())
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind the index buffer AFTER the vao has been unbound
}

void OWMeshRenderer::add(const Texture& texture)
{
	mTexture = texture;
	mTexture.samplerName("textureImageId");
}

void OWMeshRenderer::add(const MeshData& md)
{
	if (!md.v4.empty())
	{
		mMultiArrayStartIndexes.push_back(static_cast<GLsizei>(mData.v4.size()));
		mData.v4.insert(mData.v4.end(), md.v4.begin(), md.v4.end());
		mMultiArrayVertexCount.push_back(static_cast<GLsizei>(md.v4.size()));
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

void OWMeshRenderer::doRender() 
{
	glBindVertexArray(mVao);
	if (mSSBO.dataExists(GPUBufferObject::BufferStyle::SSBO))
	{
		// Does not appear to impact performance at all
		// Note the 1 matches our binding = 1 in the vertex shader
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mSbo);
	}
	glPolygonMode(GL_FRONT_AND_BACK, mData.polygonMode_mode);
	if (mTexture.samplerName().size() > 0)
	{
		glActiveTexture(mTexture.imageUnit());
		glBindTexture(mTexture.target(), mTexture.location());
		// associate sampler with name in shader
		//shader()->setInteger(mTexture.samplerName(), mTexture.imageUnit() - GL_TEXTURE0);
	}
	if (mMultiElementIndicesCounts.size())
	{
		// Look at:
		// https://www.reddit.com/r/opengl/comments/19bgtcb/is_the_effort_of_glmultidrawelements_worth_it/
		// If says that glMultiDrawElements is obsolete and instead use glMultiDrawElementsIndirect

		glMultiDrawElements(mData.vertexMode, mMultiElementIndicesCounts.data(), GL_UNSIGNED_INT, 
			(const void**)mMultiElementStartIndexes.data(), static_cast<GLsizei>(mMultiArrayStartIndexes.size()));
	}
	else if (mMultiArrayStartIndexes.size() == 1)
	{
		GLsizei numInstances = static_cast<GLsizei>(mSSBO.instanceCount());
		glDrawArraysInstanced(mData.vertexMode, 0,
			mMultiArrayVertexCount.back(),
			numInstances);
	}
	else
	{
		glMultiDrawArrays(mData.vertexMode, mMultiArrayStartIndexes.data(),
						mMultiArrayVertexCount.data(), static_cast<GLsizei>(mMultiArrayStartIndexes.size()));
	}
	glBindVertexArray(0);
}
