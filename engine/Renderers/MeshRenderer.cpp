#include "MeshRenderer.h"


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/Shader.h"

unsigned int OWMeshRenderer::mPrimitiveRestart = 0xFFFF;

void OWMeshRenderer::doSetup(const std::vector<OWMeshData>& meshes,
	const std::vector<OWModelData>& models)
{
	if (models.size())
		throw NMSLogicException(
			"Error. OWMeshRender cannot draw OWModelData \n");
	for (const auto& m : meshes)
	{
		add(m.meshData);
	}
	prepareOpenGL();
}

void OWMeshRenderer::add(const MeshData& meshData)
{
	RenderData rd;
	if (!meshData.v3.empty())
	{
		mVec3.insert(mVec3.end(), meshData.v3.begin(), meshData.v3.end());
		if (mDrawType == RenderType::DRAW_PRIMITIVE)
		{
			size_t begin = mPrimitiveIndices.size();
			for (size_t i = 0; i < meshData.v3.size(); i++)
				mPrimitiveIndices.push_back(i + begin);
			mPrimitiveIndices.push_back(mPrimitiveRestart);
		}
		rd.verticesCount = meshData.v3.size();
	}
	else if (!meshData.v4.empty())
	{
		mVec4.insert(mVec4.end(), meshData.v4.begin(), meshData.v4.end());
		if (mDrawType == RenderType::DRAW_PRIMITIVE)
		{
			std::vector<int> temp = std::vector<int>(meshData.v4.size(), 7);
			mPrimitiveIndices.insert(mPrimitiveIndices.end(), temp.begin(), temp.end());
			mPrimitiveIndices.push_back(mPrimitiveRestart);
		}
		rd.verticesCount = meshData.v4.size();
	}
	if (!meshData.indices.empty())
	{
		mIndices.insert(mIndices.end(), meshData.indices.begin(), meshData.indices.end());
		rd.indicesCount = sizeof(unsigned int) * static_cast<GLsizei>(meshData.indices.size());
	}
	mRenderData.push_back(rd);
}

void OWMeshRenderer::prepareOpenGL()
{
	if (mVec4.size() && mVec3.size())
		throw NMSLogicException("VAOBuffer::prepare cannot have both vec4 and vec3 arrays.");
	if (mVec4.size() == 0 && mVec3.size() == 0)
		return;
	if (mRenderData[0].mPolygonMode_mode == UINT_MAX)
		mRenderData[0].mPolygonMode_mode = GL_FILL;

	// Check for common error
	if (mRenderData[0].mPolygonMode_mode == GL_LINES)
		mRenderData[0].mPolygonMode_mode = GL_LINE;

	if (mVao == std::numeric_limits<unsigned int>::max())
		glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	shader()->use();
	if (!mRenderData[0].shaderColourName.empty())
	{
		shader()->setVector4f(mRenderData[0].shaderColourName, mRenderData[0].colour);
	}

	if (!mIndices.empty())
	{
		glGenBuffers(1, &mEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * static_cast<GLsizei>(mIndices.size()),
			mIndices.data(), GL_STATIC_DRAW);
	}
	if (mVec4.size())
	{
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0); //mData.vertexLocation);
		glBufferData(GL_ARRAY_BUFFER, mVec4.size() * 4 * sizeof(float), mVec4.data(), GL_STATIC_DRAW);
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
		glBufferData(GL_ARRAY_BUFFER, mVec3.size() * 3 * sizeof(float), mVec3.data(), GL_STATIC_DRAW);
	}
	if (mDrawType == RenderType::DRAW_PRIMITIVE)
	{
		glGenBuffers(1, &mPrimitiveEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mPrimitiveIndices.size(), 
			mPrimitiveIndices.data(), GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);
	if (mDrawType == RenderType::DRAW_MULTI)
	{
		GLint startIndex = 0;
		for (RenderData rd : mRenderData)
		{
			mMultiArrayStartIndexes.push_back(startIndex);
			startIndex += rd.verticesCount;
			mMultiArrayVertexCount.push_back(rd.verticesCount);
		}
	}

	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	if (!mIndices.empty())
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind the index buffer AFTER the vao has been unbound
}

void OWMeshRenderer::doRender() 
{
	if (!mRenderData[0].shaderColourName.empty())
	{
		shader()->setVector4f(mRenderData[0].shaderColourName, mRenderData[0].colour);
	}

	glBindVertexArray(mVao);
	if (mRenderData[0].indicesCount)
	{
		glDrawElements(mRenderData[0].indicesMode,
			static_cast<GLsizei>(mRenderData[0].indicesCount),
			GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(mRenderData[0].vertexMode, 0, static_cast<GLsizei>(mRenderData[0].verticesCount));
	}
	glBindVertexArray(0);
}
