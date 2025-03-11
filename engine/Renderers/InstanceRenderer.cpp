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


void OWInstanceRenderer::doSetup(const OWRenderData& rd)
{
	if (rd.meshes.size())
	{
		for (const auto& m: rd.meshes)
			setupMesh(m);
	}

	if (rd.instances.size())
	{
		if (rd.instances.size() != 1)
			throw NMSLogicException("InstanceRenderer::doSetup cannot handle multiple Instance Data. Cannot recover.");

		for (const auto& m : rd.instances)
		{
			unsigned int vertexSize = 0;
			validate(m);
			if (!m.v3.empty())
			{
				mData.v3.insert(mData.v3.end(), m.v3.begin(), m.v3.end());
				mVerticeCount = static_cast<GLsizei>(mData.v3.size());
				vertexSize = 3;
			}
			else if (!m.v4.empty())
			{
				mData.v4.insert(mData.v4.end(), m.v4.begin(), m.v4.end());
				mVerticeCount = static_cast<GLsizei>(m.v4.size());
				vertexSize = 4;
			}
			mData.instancePositions.insert(mData.instancePositions.end(), m.instancePositions.begin(), m.instancePositions.end());
			mPositionCount += static_cast<GLsizei>(mData.instancePositions.size());

			mData.instanceColours.insert(mData.instanceColours.end(), m.instanceColours.begin(), m.instanceColours.end());
			mColourCount += static_cast<GLsizei>(mData.instanceColours.size());

			mData.vertexMode = m.vertexMode;
			mData.vertexLocation = m.vertexLocation;

			// Instancing Data
			mData.positionDivisor = m.positionDivisor;
			mData.positionLocation = m.positionLocation;
			mData.colourDivisor = m.colourDivisor;
			mData.colourLocation = m.colourLocation;

		}
		setupInstance();
	}

	if (rd.models.size())
	{
		for (const auto& m : rd.models)
			setupModel(m);
	}
}

void OWInstanceRenderer::setupModel(const OWModelData& model)
{
	throw NMSLogicException("InstanceRenderer::doSetup cannot handle Models. Cannot recover.");
}

void OWInstanceRenderer::setupInstance()
{
	validateBase();
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(3, &mVbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(mData.vertexLocation,
		mVerticeCount, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	const float* ff = nullptr;
	if (mData.v3.size())
	{
		// Very clunky
		const glm::vec3* p = mData.v3.data();
		ff = glm::value_ptr(*p);
	}
	else
	{
		const glm::vec4* p = mData.v4.data();
		ff = glm::value_ptr(*p);
	}
	glBufferData(GL_ARRAY_BUFFER,
		mVerticeCount * mVerticeCount * sizeof(float), ff, GL_STATIC_DRAW);

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
	mPositionCount = static_cast<GLsizei>(mData.instancePositions.size());
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mPositionCount,
		mData.instancePositions.data(), GL_STREAM_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mData.instanceColours.size(),
		mData.instanceColours.data(), GL_STREAM_DRAW);

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

void OWInstanceRenderer::setupMesh(const MeshData& mesh)
{
	throw NMSLogicException("InstanceRenderer::doSetup cannot handle MeshData. Cannot recover.");
}

void OWInstanceRenderer::doRender() 
{
	glBindVertexArray(mVao);

	// Draw the particles !
	// This draws many times a small triangle_strip (which looks like a quad).
	// This is equivalent to :
	// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
	// but faster.
	// Updates gl_InstanceID which can be accessed in the vertex shader

	// Good tutorial on SSBO's
	// https://ktstephano.github.io/rendering/opengl/ssbos
	// https://stackoverflow.com/questions/39707463/instancing-using-more-values-than-uniforms-can-store
	// A good linke for textures sprites
	// https://www.reddit.com/r/opengl/comments/yi012y/best_practices_for_rendering_many_constantly/
	glDrawArraysInstanced(mData.vertexMode, 0,
			static_cast<GLsizei>(mVerticeCount),
			static_cast<GLsizei>(mPositionCount));
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
	if (meshData.positionDivisor == UINT_MAX)
		throw NMSLogicException("InstanceRenderer missing position divisor");
	if (meshData.colourDivisor == UINT_MAX)
		throw NMSLogicException("InstanceRenderer missing colour divisor");

}
