#include "ParticlesRenderer.h"

#include "../Helpers/ErrorHandling.h"

#include "Particles.h"
#include "../Helpers/Shader.h"

ParticlesRenderer::ParticlesRenderer()
{}

// Basically code pasted from:
//http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/

// This has some info as well
// https://www.informit.com/articles/article.aspx?p=2033340&seqNum=5

// One buffer for the vertices of the mesh.No index buffer, 
//		so it’s 6 vec3, which make 2 triangles, which make 1 quad.
// One buffer for the particles’ centers.
// One buffer for the particles’ colors.


void ParticlesRenderer::prepare(const Particles* source)
{
	checkSourceForErrors(source);
	//// The VBO containing the 4 vertices of the particles.
	//// Thanks to instancing, they will be shared by all particles.
	//static const GLfloat g_vertex_buffer_data[] = {
	// -0.5f, -0.5f, 0.0f,
	// 0.5f, -0.5f, 0.0f,
	// -0.5f, 0.5f, 0.0f,
	// 0.5f, 0.5f, 0.0f,
	//};
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
	glGenBuffers(3, &mVbo[0]);

	// The triangles to draw
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
	glVertexAttribPointer(
		source->mVertexLoc, // attribute. must match the layout in the shader.
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * source->mVec3.size(),
				source->mVec3.data(), GL_STATIC_DRAW);

	// The positions
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
	if (source->mPositionsV3.size())
	{
		glVertexAttribPointer(
			source->mPositionLocation, // must match the layout in the shader.
			3, // size : x + y + z => 3
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
		);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * source->mPositionsV3.size(),
			source->mPositionsV3.data(), GL_STREAM_DRAW);
	}
	else if (source->mPositionsV4.size())
	{
		glVertexAttribPointer(
			source->mPositionLocation, // must match the layout in the shader.
			4, // size : x + y + z + w => 4
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
		);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * source->mPositionsV4.size(),
			source->mPositionsV4.data(), GL_STREAM_DRAW);
	}

	// The colours
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[2]);
	glVertexAttribPointer(
		source->mColourLocation, // must match the layout in the shader.
		4, // size : r + g + b + a => 4
		GL_FLOAT, // type
		GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0, // stride
		(void*)0 // array buffer offset
	);
	glEnableVertexAttribArray(2);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * source->mColours.size(), 
				source->mColours.data(), GL_STREAM_DRAW);

	glBindVertexArray(0);
}

void ParticlesRenderer::render(const Particles* source,
	const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	OWUtils::RenderCallbackType renderCb,
	OWUtils::ResizeCallbackType resizeCb) const
{
	OWUtils::PolygonModeRIAA poly;
	source->mShader->use();
	setPVM(source, proj, view, model);

	glBindVertexArray(mVao);
	callResizeCallback(source, resizeCb);
	callRenderCallback(source, proj, view, model, renderCb);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes 
	// advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml

	// particles vertices : always reuse the same 3 vertices -> 0
	glVertexAttribDivisor(source->mVertexLoc, 0); 

	// positions : one per quad (its center) -> 1
	glVertexAttribDivisor(source->mPositionLocation, source->mPositionDivisor); 

	// color : 
	glVertexAttribDivisor(source->mColourLocation, source->mColourDivisor); 

	// Draw the particles !
	// This draws many times a small triangle_strip (which looks like a quad).
	// This is equivalent to :
	// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
	// but faster.
	if (source->mPositionsV3.size())
	{
		if (source->mVec3.size())
		{
			glDrawArraysInstanced(source->mVertexMode, 0,
				static_cast<GLsizei>(source->mVec3.size()),
				static_cast<GLsizei>(source->mPositionsV3.size()));
		}
		else
		{
			glDrawArraysInstanced(source->mVertexMode, 0,
				static_cast<GLsizei>(source->mVec4.size()),
				static_cast<GLsizei>(source->mPositionsV3.size()));
		}
	}
	else if (source->mPositionsV4.size())
	{
		if (source->mVec3.size())
		{
			glDrawArraysInstanced(source->mVertexMode, 0,
				static_cast<GLsizei>(source->mVec3.size()),
				static_cast<GLsizei>(source->mPositionsV4.size()));
		}
		else
		{
			glDrawArraysInstanced(source->mVertexMode, 0,
				static_cast<GLsizei>(source->mVec4.size()),
				static_cast<GLsizei>(source->mPositionsV4.size()));
		}
	}
}

void ParticlesRenderer::checkSourceForErrors(const Particles* source)
{
	checkBaseSourceForErrors(source);
	if (source->mVec3.size() == 0)
		throw NMSLogicException("mVec3 must be used for ParticlesRenderer");
	if (source->mVec4.size() != 0)
		throw NMSLogicException("mVec4 must not be used for ParticlesRenderer");
	if (source->mPositionsV3.size() && source->mPositionsV4.size())
		throw NMSLogicException("source->mPositions. Particles must have only have one set of positions");
	if (source->mPositionsV3.size() == 0 && source->mPositionsV4.size() == 0)
		throw NMSLogicException("source->mPositions. Particles must have at least one set of positions");
	if (source->mColours.size() == 0)
		throw NMSLogicException("source->mColours. Particles must have colours");
	if (source->mPositionMode == GL_INVALID_ENUM)
		throw NMSLogicException("source->mPositionMode. Particles position must have a mode");
	if (source->mPositionDivisor == GL_INVALID_INDEX)
		throw NMSLogicException("source->mPositionMode. Particles position divisor must be set");
	if (source->mColourDivisor== GL_INVALID_INDEX)
		throw NMSLogicException("source->mColourDivisor. Particles colour divisor must be set");
}

