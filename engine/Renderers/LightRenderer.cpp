#include "LightRenderer.h"


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/Shader.h"

void LightRenderer::setup(const MeshDataLight* meshData)
{
	const float* ff = nullptr;
	unsigned int vertexSize = 0;
	validate(meshData);
	mData = meshData->mRenderData;
	if (!meshData->mVec3.empty())
	{
		const glm::vec3* p = meshData->mVec3.data();
		ff = glm::value_ptr(*p);
		mData.verticesCount = meshData->mVec3.size();
		vertexSize = 3;
	}
	else if (!meshData->mVec4.empty())
	{
		const glm::vec4* p = meshData->mVec4.data();
		ff = glm::value_ptr(*p);
		mData.verticesCount = meshData->mVec4.size();
		vertexSize = 4;
	}
	validateBase();
	mData.vertexMode = mData.vertexMode;
	mData.vertexLocation = mData.vertexLocation;
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	shader()->use();
	if (!mData.shaderColourName.empty())
	{
		shader()->setVector4f(mData.shaderColourName, mData.colour);
	}

	if (!meshData->mIndices.empty())
	{
		glGenBuffers(1, &mEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * static_cast<GLsizei>(meshData->mIndices.size()),
			meshData->mIndices.data(), GL_STATIC_DRAW);
	}
	glVertexAttribPointer(mData.vertexLocation,
		vertexSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(mData.vertexLocation);
	glBufferData(GL_ARRAY_BUFFER,
		mData.verticesCount * vertexSize * sizeof(float), ff, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void LightRenderer::setup(const std::vector<glm::vec3>& v,
	unsigned int vertexMode, unsigned int vertexLocation)
{
	MeshDataLight mdl;
	mdl.mVec3 = v;
	mdl.mRenderData.verticesCount = v.size();
	mdl.mRenderData.indicesCount = 0;
	mdl.mRenderData.indicesMode = 0;
	mdl.mRenderData.vertexMode = vertexMode;
	mdl.mRenderData.vertexLocation = vertexLocation;
	setup(&mdl);
}

void LightRenderer::setup(const std::vector<glm::vec4>& v,
	unsigned int vertexMode, unsigned int vertexLocation)
{
	MeshDataLight mdl;
	mdl.mVec4 = v;
	mdl.mRenderData.verticesCount = v.size();
	mdl.mRenderData.indicesCount = 0;
	mdl.mRenderData.indicesMode = 0;
	mdl.mRenderData.vertexMode = vertexMode;
	mdl.mRenderData.vertexLocation = vertexLocation;
	setup(&mdl);
}

void LightRenderer::doRender() const
{
	if (!mData.shaderColourName.empty())
	{
		constShader()->setVector4f(mData.shaderColourName, mData.colour);
	}

	glBindVertexArray(mVao);
	if (mData.indicesCount)
	{
		glDrawElements(mData.indicesMode, 
				static_cast<GLsizei>(mData.indicesCount), 
				GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(mData.vertexMode, 0, static_cast<GLsizei>(mData.verticesCount));
	}
	glBindVertexArray(0);
}

void LightRenderer::validate(const MeshDataLight* mdl)
{
	validateBase();
	if (mdl->mVec4.empty() == mdl->mVec3.empty())
	{
		if (mdl->mVec4.empty())
			throw NMSLogicException("LightRenderer has no data");
		else
			throw NMSLogicException("LightRenderer has v3 and v4 data");
	}
}
