#include "OpenInfo.h"

#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OpenInfo::use()
{
	for (auto& elm : objects)
	{
		elm->use();
	}
}

void OpenInfo::cleanup()
{
	for (auto& elm : objects)
	{
		elm->cleanup();
	}
}

void OpenInfoObject::configure()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	if (indices.size())
	{
		glGenBuffers(1, &ebo);
	}
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size() * sizeof(float), 0, GL_STATIC_DRAW);

	if (triangles.size())
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, triangles[0].size() * sizeof(float), triangles[0].data());
	}
	for (size_t i = 1; i < triangles.size(); i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, triangles[i-1].size() * sizeof(float), 
								triangles[1].size() * sizeof(float), 
								triangles[i].data());
	}

	if (indices.size())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	// position attributes
	for (int i = 0; i < attrPointers.size(); i++)
	{
		std::function<void(int)> attr = attrPointers[i];
		attr(i);
		glEnableVertexAttribArray(i);
	}

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex 
	// attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways 
	// so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	shader.configure();
}

unsigned int OpenInfoObject::size() const
{
	size_t retval = 0;
	for (auto& tri : triangles)
	{
		retval += tri.size();
	}
	return (unsigned int)retval;
}

void OpenInfo::polygonDrawMode(int face, int mode)
{
	glPolygonMode(face, mode);
}

void OpenInfo::clearColour(int buffer, float red, float green, float blue, float alpha)
{
	glClearColor(red, green, blue, alpha);
	glClear(buffer);
}

void OpenInfoObject::use()
{
	texture.use();
	shader.use();
	// update the uniform color
	float timeValue = static_cast<float>(glfwGetTime());
	float greenValue = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shader.shaderProgram(), "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	// seeing as we only have a single VAO there's no need to bind it 
	// every time, but we'll do so to keep things a bit more organized
	glBindVertexArray(vao);
	if (indices.size())
	{
		glDrawElements(GL_TRIANGLES, size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, size());
	}
}

void OpenInfoObject::cleanup()
{
	glDeleteVertexArrays(1, &(vao));
	glDeleteBuffers(1, &(vbo));
	if (ebo)
		glDeleteBuffers(1, &(ebo));

	shader.cleanUp();
}
