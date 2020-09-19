#include "Renderer.h"

#include <glm/gtx/transform.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../Helpers/Shader.h"
#include "../Helpers//ResourceFactory.h"

Renderer::Renderer(Shader* _shader)
	:mShader(_shader)
{
	//assert(false);
	if (mShader == nullptr)
	{
		mShader = new Shader();
		mShader->loadShaders(ResourceFactory::boilerPlateVertexShader(),
						 	 ResourceFactory::boilerPlateFragmentShader(),
							 ResourceFactory::boilerPlateGeometryShader());
	}
}

void Renderer::prepareOpenGL(const std::vector<glm::vec3>& vertices,
					   const std::vector<unsigned int>& indices,
					   const glm::vec4& colour)
{
	mShader->use();
	mShader->setVector4f("colour", colour);

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);
	if (!indices.empty())
	{
		glGenBuffers(1, &mEbo);
	}

	// bind the Vertex Array Object first, then bind and set 
	// vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sizeof(vertices),
					vertices.data(), GL_STATIC_DRAW);

	if (!indices.empty())
	{
		mIndicesSize = indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
				sizeof(unsigned int) * mIndicesSize, 
				indices.data(), GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer 
	// registered VBO as the vertex attribute's bound vertex buffer 
	// object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't 
	// accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't 
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void Renderer::renderOpenGL(const glm::mat4& proj,
				const glm::mat4& view,
				const glm::mat4& model,
				unsigned int type,
				std::function<void()> cb) const
{
	// draw our first triangle
	mShader->use();
	glm::mat4 pvm = proj * view * model;
	mShader->setMatrix4("pvm", pvm);

	glBindVertexArray(mVao);
	if (cb)
		cb();
	if (mIndicesSize)
	{
		glDrawElements(type, static_cast<GLsizei>(mIndicesSize), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(type, 0, 3);
	}
	glBindVertexArray(0);
}
