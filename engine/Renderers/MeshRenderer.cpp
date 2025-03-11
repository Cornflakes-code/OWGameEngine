#include "MeshRenderer.h"


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/Shader.h"

unsigned int OWMeshRenderer::mPrimitiveRestart = 0xFFFF;

void OWMeshRenderer::doSetup(const OWRenderData& renderData)
{
	if (renderData.models.size())
		throw NMSLogicException(
			"Error. OWMeshRender cannot draw OWModelData \n");
	if (renderData.textures.size() > 1000000) // jfw fix me. The textures may be identical.
	{
		throw NMSLogicException(
			"Error. OWMeshRender cannot handle more than one texture.\n");
	}

	// if (renderData.textures.size() == 1) jfw fix me
		add(renderData.textures[0]);
	for (const auto& m : renderData.meshes)
	{
		add(m);
	}
	prepareOpenGL();
}

void OWMeshRenderer::add(const Texture& texture)
{
	mTexture = texture;
	mTexture.samplerName("textureImageId");

}

void OWMeshRenderer::add(const MeshData& meshData)
{
	mNumMeshes++;
	if (!meshData.v3.empty())
	{
		mMultiArrayStartIndexes.push_back(static_cast<GLsizei>(mData.v3.size()));
		mData.v3.insert(mData.v3.end(), meshData.v3.begin(), meshData.v3.end());
		mMultiArrayVertexCount.push_back(static_cast<GLsizei>(meshData.v3.size()));
		if (mDrawType == RenderType::DRAW_PRIMITIVE)
		{
			size_t begin = mPrimitiveIndices.size();
			for (GLsizei i = 0; i < meshData.v3.size(); i++)
				mPrimitiveIndices.push_back(static_cast<GLsizei>(i + begin));
			mPrimitiveIndices.push_back(mPrimitiveRestart);
		}
	}
	else if (!meshData.v4.empty())
	{
		mMultiArrayStartIndexes.push_back(static_cast<GLsizei>(mData.v4.size()));
		mData.v4.insert(mData.v4.end(), meshData.v4.begin(), meshData.v4.end());
		mMultiArrayVertexCount.push_back(static_cast<GLsizei>(meshData.v4.size()));
		if (mDrawType == RenderType::DRAW_PRIMITIVE)
		{
			std::vector<int> temp = std::vector<int>(meshData.v4.size(), 7);
			mPrimitiveIndices.insert(mPrimitiveIndices.end(), temp.begin(), temp.end());
			mPrimitiveIndices.push_back(mPrimitiveRestart);
		}
	}
	if (!meshData.indices.empty())
	{
		// https://stackoverflow.com/questions/24516993/is-it-possible-to-use-index-buffer-objects-ibo-with-the-function-glmultidrawe
		mMultiElementStartIndexes.push_back(static_cast<GLsizei>(mData.indices.size() * sizeof(GLsizei)));
		mData.indices.insert(mData.indices.end(), meshData.indices.begin(), meshData.indices.end());
		mMultiElementIndicesCounts.push_back(static_cast<GLsizei>(mData.indices.size()));
	}
	mData.shaderColourName = meshData.shaderColourName;
	mData.colour = meshData.colour;
	mData.vertexMode = meshData.vertexMode;
	mData.vertexLocation = meshData.vertexLocation;
	mData.indicesMode = meshData.indicesMode;
	mData.polygonMode_mode = meshData.polygonMode_mode;
}

void OWMeshRenderer::prepareOpenGL()
{
	if (mData.v3.size() && mData.v4.size())
		throw NMSLogicException("OWMeshRenderer::prepareOpenGL cannot have both vec4 and vec3 arrays.");
	if (mData.v3.size() == 0 && mData.v4.size() == 0)
		return;
	if (mData.polygonMode_mode == UINT_MAX)
		mData.polygonMode_mode = GL_FILL;

	// Check for common error
	if (mData.polygonMode_mode == GL_LINES)
		mData.polygonMode_mode = GL_LINE;

	if (mSSBO.size > 0)
	{
		if (true)
		{
			glCreateBuffers(1, &mSbo);
			glNamedBufferStorage(mSbo,
				mSSBO.size,
				(const void*)mSSBO.data,
				GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
			// Note the 1 matches our binding = 2 in the vertex shader
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, mSbo);
		}
		else
		{
			glGenBuffers(1, &mSbo);
			glNamedBufferStorage(mSbo,
				mSSBO.size,
				(const void*)mSSBO.data,
				GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
		}
	}
	if (mVao == std::numeric_limits<unsigned int>::max())
		glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
	if (mSSBO.size > 0)
	{
		// Note the 1 matches our binding = 1 in the vertex shader
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mSbo);
	}
	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	shader()->use();
	if (!mData.shaderColourName.empty())
	{
		shader()->setVector4f(mData.shaderColourName, mData.colour);
	}

	if (!mData.indices.empty())
	{
		glGenBuffers(1, &mEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * static_cast<GLsizei>(mData.indices.size()),
			mData.indices.data(), GL_STATIC_DRAW);
	}
	unsigned int vertexLoc = shader()->getAttributeLocation("coord");
	if (mData.v4.size())
	{
		glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(vertexLoc);
		unsigned int sz = mData.v4.size() * 4 * sizeof(float);
		glBufferData(GL_ARRAY_BUFFER, mData.v4.size() * 4 * sizeof(float), mData.v4.data(), GL_STATIC_DRAW);
	}
	else
	{
		// Youtube video explaning glVertexAttribPointer
		// https://www.youtube.com/watch?v=kQOwkG15dYo
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		// Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float)));
		glBufferData(GL_ARRAY_BUFFER, mData.v3.size() * 3 * sizeof(float), mData.v3.data(), GL_STATIC_DRAW);
	}
	if (mDrawType == RenderType::DRAW_PRIMITIVE)
	{
		glGenBuffers(1, &mPrimitiveEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mPrimitiveIndices.size(), 
			mPrimitiveIndices.data(), GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);

	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (!mData.indices.empty())
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind the index buffer AFTER the vao has been unbound
}

void OWMeshRenderer::doRender() 
{
	if (!mData.shaderColourName.empty())
	{
		shader()->setVector4f(mData.shaderColourName, mData.colour);
	}

	glBindVertexArray(mVao);
	//glPolygonMode(GL_FRONT_AND_BACK, mData.polygonMode_mode);
	if (mTexture.samplerName().size() > 0)
	{
		glActiveTexture(mTexture.imageUnit());
		glBindTexture(mTexture.target(), mTexture.location());
		// associate sampler with name in shader
		//shader()->setInteger(mTexture.samplerName(), mTexture.imageUnit() - GL_TEXTURE0);
	}
	if (mDrawType == RenderType::DRAW_PRIMITIVE)
	{
		// FPS = 27/28
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveEbo);
		checkGLError();
		glDrawElements(mData.vertexMode, static_cast<GLsizei>(mPrimitiveIndices.size()), GL_UNSIGNED_INT, 0);
		checkGLError();
	}
	if (mDrawType == RenderType::DRAW_MULTI)
	{
		if (mMultiElementIndicesCounts.size())
		{
			// Look at:
			// https://www.reddit.com/r/opengl/comments/19bgtcb/is_the_effort_of_glmultidrawelements_worth_it/
			// If says that glMultiDrawElements is obsolete and instead use glMultiDrawElementsIndirect
			GLsizei* x = mMultiElementStartIndexes.data();
			const void* xx = static_cast<void*>(x);
			glMultiDrawElements(mData.vertexMode, mMultiElementIndicesCounts.data(),  GL_UNSIGNED_INT, &xx, mNumMeshes);
		}
		else
		{
			// jfw fix me
			//glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mData.v4.size()));

			glMultiDrawArrays(mData.vertexMode, mMultiArrayStartIndexes.data(), mMultiArrayVertexCount.data(), mNumMeshes);
		}
	}
	glBindVertexArray(0);
}
