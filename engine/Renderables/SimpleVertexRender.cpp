#include "SimpleVertexRender.h"

#include <cassert>
#include "../Helpers/Shader.h"

#include "SimpleVertexSource.h"
#include "../Helpers/ErrorHandling.h"

void SimpleVertexRender::setUp(SimpleVertexSource* _source)
{
	mSource = _source;
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	mSource->shader()->use();

	std::string vertLoc;
	unsigned int vertType = -1;
	const std::vector<glm::vec4>& vertices = mSource->vertices(vertLoc, vertType);
	assert(vertType != -1);
	unsigned int indexType = -1;
	const std::vector<unsigned int >& indices = mSource->indices(indexType);
	assert(indexType != -1);
	if (!indices.empty())
	{
		glGenBuffers(1, &mEbo);
	}
	GLuint location = mSource->shader()->getAttributeLocation(vertLoc);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

	glEnableVertexAttribArray(location);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec4) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	if (!indices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * indices.size(),
			indices.data(), GL_STATIC_DRAW);
	}
	checkGLError();
	glBindVertexArray(0);
}

void SimpleVertexRender::render(const glm::mat4& proj,
								const glm::mat4& view,
								const glm::mat4& model) const
{
	/*
	Links to Texture Bindings
	https://stackoverflow.com/questions/57621863/opengl-some-clarification-on-using-multiple-textures-or-texture-units

	Billboards
	// http://ogldev.org/www/tutorial27/tutorial27.html
	// http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/
	// https://www.braynzarsoft.net/viewtutorial/q16390-36-billboarding-geometry-shader
	// E:\Apps\OpenGL\ogl-master\tutorial18_billboards_and_particles
	*/
	glBindVertexArray(mVao);
	GLuint texture = mSource->texture();
	if (texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	mSource->doRender(proj, view, model);
	/*
		if I pass GL_TEXTURE0 to TextBillboard then I must set textureUnit = 0.
		if I pass GL_TEXTURE1 to TextBillboard then I must set textureUnit = 1.
		see https://stackoverflow.com/questions/14231391/what-is-the-function-of-glactivetexture-and-gl-texture0-in-opengl
		and https://www.khronos.org/opengl/wiki/Example_Code
	*/
	checkGLError();
	unsigned int indexType;
	const std::vector<unsigned int>& indices = mSource->indices(indexType);
	assert(indexType != -1);

	if (!indices.empty())
	{
		glDrawElements(indexType, static_cast<GLsizei>(indices.size()), 
						GL_UNSIGNED_INT, 0);
	}
	else
	{
		std::string vertLoc;
		unsigned int vertType = -1;
		const std::vector<glm::vec4>& vertices = mSource->vertices(vertLoc, vertType);
		assert(vertType != -1);
		glDrawArrays(vertType, 0, static_cast<GLsizei>(vertices.size()));
	}
	checkGLError();
	glBindVertexArray(0);
	if (texture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
