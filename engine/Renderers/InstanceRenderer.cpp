#include "InstanceRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/Shader.h"


// Basically code pasted from:
//http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/

// This has some info as well
// https://www.informit.com/articles/article.aspx?p=2033340&seqNum=5

// One buffer for the vertices of the mesh.No index buffer, 
//		so it’s 6 vec3, which make 2 triangles, which make 1 quad.
// One buffer for the particles’ centers.
// One buffer for the particles’ colors.


void OWInstanceRenderer::doSetup(const std::vector<OWMeshData>& meshes,
	const std::vector<OWModelData>& models)
{
	if (meshes.size())
		setupMeshes(meshes);
	else
		throw NMSLogicException("Meshes should not be empty for OWInstanceRenderer::doSetup. Cannot recover.");

	if (models.size())
		setupModels(models);
}

void OWInstanceRenderer::setupModels(const std::vector<OWModelData>& models)
{
	throw NMSLogicException("Models not yet implmeneted for OWInstanceRenderer::doSetup.Cannot recover.");
}


void OWInstanceRenderer::setupMeshes(const std::vector<OWMeshData>& meshes)
{
	const float* ff = nullptr;
	unsigned int vertexSize = 0;
	const InstanceData& mdi = meshes[0].instanceData;
	validate(mdi);
	mData = mdi.renderData;
	if (!mdi.v3.empty())
	{
		const glm::vec3* p = mdi.v3.data();
		ff = glm::value_ptr(*p);
		mData.verticesCount = mdi.v3.size();
		vertexSize = 3;
	}
	else if (!mdi.v4.empty())
	{
		const glm::vec4* p = mdi.v4.data();
		ff = glm::value_ptr(*p);
		mData.verticesCount = mdi.v4.size();
		vertexSize = 4;
	}
	validateBase();
	mData.vertexMode = mData.vertexMode;
	mData.vertexLocation = mData.vertexLocation;
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(3, &mVbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(mData.vertexLocation,
		vertexSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBufferData(GL_ARRAY_BUFFER,
		mData.verticesCount * vertexSize * sizeof(float), ff, GL_STATIC_DRAW);

	// The positions
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
	glVertexAttribPointer(
		mData.positionLocation, // must match the layout in the shader.
		3, // size : x + y + z => 3
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mData.positionCount,
		mdi.instancePositions.data(), GL_STREAM_DRAW);
	// The colours
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[2]);
	glVertexAttribPointer(
		mData.colourLocation, // must match the layout in the shader.
		4, // size : r + g + b + a => 4
		GL_FLOAT, // type
		GL_TRUE, // normalized? *** YES, this means that the 
				 // unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0, // stride
		(void*)0 // array buffer offset
	);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mdi.instanceColours.size(),
		mdi.instanceColours.data(), GL_STREAM_DRAW);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes 
	// advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml

	// particles vertices : always reuse the same vertices -> 0
	glVertexAttribDivisor(mData.vertexLocation, 0);

	// positions : one per quad (its center) -> 1
//	glVertexAttribDivisor(mData.positionLocation, mData.positionDivisor);
	glVertexAttribDivisor(mData.positionLocation, 1);

	// color : 
	//glVertexAttribDivisor(mData.colourLocation, mData.colourDivisor);
 	glVertexAttribDivisor(mData.colourLocation, 1);


	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void OWInstanceRenderer::doRender() 
{
	glBindVertexArray(mVao);

	// Draw the particles !
	// This draws many times a small triangle_strip (which looks like a quad).
	// This is equivalent to :
	// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
	// but faster.
	glDrawArraysInstanced(mData.vertexMode, 0,
			static_cast<GLsizei>(mData.verticesCount),
			static_cast<GLsizei>(mData.positionCount));
}

void OWInstanceRenderer::validate(const InstanceData& meshData) const
{
	validateBase();
	if (meshData.v3.empty())
		throw NMSLogicException("InstanceRenderer missing vertices");
	if (meshData.instancePositions.empty())
		throw NMSLogicException("InstanceRenderer missing positions");
	if (meshData.instanceColours.empty())
		throw NMSLogicException("InstanceRenderer missing colours");
	if (meshData.renderData.positionDivisor == UINT_MAX)
		throw NMSLogicException("InstanceRenderer missing position divisor");
	if (meshData.renderData.colourDivisor == UINT_MAX)
		throw NMSLogicException("InstanceRenderer missing colour divisor");

}
