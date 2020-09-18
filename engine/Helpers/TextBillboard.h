#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>


#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Core/GLApplication.h"
#include "../Core/ListenerHelper.h"

#include "../Helpers/CommonUtils.h"
#include "../Renderables/BoundingBox.h"

#include "../Renderables/SimpleVertexSource.h"
#include "ResourceSource.h"
#include "ResourceFactory.h"
#include "ResizeHelper.h"

class Shader;

/*
	Base class for fixed and dynamic Billboard Text. Font texture loading is cached
	by FreeTypeFontAtlas via ResourceManager. Rendering is done in SimpleVCertexRender
*/
class TextBillboard :
		public ResourceSource, 
		public ResizeHelper, 
		public SimpleVertexSource
{
protected:
	TextBillboard(Shader* shader, const std::string& fontFileName, int fontHeight);
public:
	~TextBillboard();
	void createText(const std::string& text, float sx, float sy);
	void color(const glm::vec4& newValue) { mColor = newValue; }
	void scale(const glm::vec2& newValue) { mScale = newValue; }
	const AABB& bounds() const { return mBounds; }

	GLuint texture() const override { return mFontData->texture(); }
	const Shader* shader() const override { return mShader; }
	std::vector<glm::vec4> vertices(std::string& verticeLocation,
									unsigned int& type) const override
	{
		type = GL_TRIANGLES;
		verticeLocation = "coord";
		return mVertices; 
	}
	std::vector<unsigned int> indices(unsigned int& type) const override
	{
		type = 0;
		return std::vector<unsigned int>();
	}

protected:
	AABB mBounds;
	glm::vec4 mColor;
	glm::vec2 mScale = { 0.5f, 0.5f };
	Shader* mShader;
	std::vector<glm::vec4> mVertices;
	const FreeTypeFontAtlas::FontDetails* mFontData;
private:
	AABB findBounds() const;
};
