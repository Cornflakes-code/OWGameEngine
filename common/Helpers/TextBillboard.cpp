#include "TextBillboard.h"

#include <iostream>
#include <algorithm>
#include <limits>
#include <experimental/filesystem>
#include <functional>

//#define GLM_FORCE_RADIANS

#include <glm/gtx/rotate_vector.hpp>

#include "ResourceFactory.h"
#include "ErrorHandling.h"
#include "Shader.h"

TextBillboard::TextBillboard(Shader* shader, const std::string& fileName, int fontHeight)
	: mShader(shader)
{
	mFontData = factory()->loadFreeTypeFont(fileName, fontHeight);

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	mShader->use();
	GLuint location = mShader->getAttributeLocation("coord");
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
	glEnableVertexAttribArray(location);
	glBindVertexArray(0);
}

TextBillboard::~TextBillboard()
{
	theApp->removeWindowResizeListener(this);
	delete mShader;
}

void TextBillboard::createText(const std::string& text, float sx, float sy)
{
	mVertices = mFontData->createText(text, sx, sy);
	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	checkGLError();
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec4) * mVertices.size(),
		mVertices.data(), GL_STATIC_DRAW);
	checkGLError();
	glBindVertexArray(0);
	auto cb = std::bind(&TextBillboard::resizeCallback, this,
		std::placeholders::_1, std::placeholders::_2);
	theApp->addWindowResizeListener(cb, this);
	mBounds = findBounds();
	// move to origin ...
	mBounds = mBounds - mBounds.minPoint();
}

AABB TextBillboard::findBounds() const
{
	// Note: No depth value: default to 0;
	const float _max = std::numeric_limits<float>::max();
	glm::vec4 minPoint(_max, _max, 0, 1);
	glm::vec4 maxPoint(-_max, -_max, 0, 1);

	for (const auto& point : mVertices)
	{
		if (point.x < minPoint.x)
			minPoint.x = point.x;
		else if (point.x > maxPoint.x)
			maxPoint.x = point.x;
		else if (point.y < minPoint.y)
			minPoint.y = point.y;
		else if (point.y > maxPoint.y)
			maxPoint.y = point.y;
	}
	return AABB(minPoint, maxPoint);
}

void TextBillboard::resizeCallback(	
				GLApplication::WindowResizeType resizeType, glm::ivec2 dimensions)
{
	float x = mRawScale.x;
	float y = mRawScale.y;
	mAspectRatioInvalidated = true;
	/*
	This seems to be better. There is a lot of interaction here between:
	1. This function.
	2. call to setViewport in Movie.cpp
	3. camera projection call passed to render.
	*/
	if (dimensions.x > dimensions.y)
	{
		x = x * dimensions.y / dimensions.x;
	}
	else
	{
		y = y * dimensions.x / dimensions.y;
	}
	mScale = { x, y };
}

void TextBillboard::render(const glm::mat4& proj, const glm::mat4& view, 
							const glm::mat4& model)
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFontData->texture());

	doRender(proj, view, model);
	/*
		if I pass GL_TEXTURE0 to TextBillboard then I must set textureUnit = 0.
		if I pass GL_TEXTURE1 to TextBillboard then I must set textureUnit = 1.
		see https://stackoverflow.com/questions/14231391/what-is-the-function-of-glactivetexture-and-gl-texture0-in-opengl
		and https://www.khronos.org/opengl/wiki/Example_Code
	*/
	checkGLError();
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mVertices.size());
	checkGLError();
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
