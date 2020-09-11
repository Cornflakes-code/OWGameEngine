#pragma once

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <map>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ResourceSource.h"
#include "./Renderables/BoundingBox.h"
#include <Core/GLApplication.h>
#include "../Helpers/CommonUtils.h"
#include "ResourceFactory.h"

class Shader;

class TextBillboard : public ResourceSource, public ListenerHelper
{
protected:
	TextBillboard(Shader* shader, const std::string& fileName, int fontHeight);
public:
	~TextBillboard();
	void color(const glm::vec4& newValue) { mColor = newValue; }
	void createText(const std::string& text, float sx, float sy);
	void rawScaling(const glm::vec2& newValue) { mRawScale = newValue; }
	glm::vec2 rawScaling() const { return mRawScale; }
	glm::vec2 drawScaling() const { return mScale; }
	void render(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& model);
	const AABB& bounds() const { return mBounds; }
protected:
	AABB mBounds;
	const std::vector<glm::vec4>& vertices() const { return mVertices; }
	virtual void doRender(const glm::mat4& proj, 
						  const glm::mat4& view,
						  const glm::mat4& model) = 0;
	Shader* mShader;
	glm::vec4 mColor;
	bool mAspectRatioInvalidated = true;
private:
	AABB findBounds() const;
	void resizeCallback(GLApplication::WindowResizeType resizeType, glm::ivec2 dimensions);
	glm::vec2 mRawScale = { 0.5, 0.5 };
	glm::vec2 mScale = { 0.5, 0.5 };

	const FreeTypeFontAtlas::FontDetails* mFontDetails;
	GLuint mVao;
	GLuint vbo;
	std::vector<glm::vec4> mVertices;
	const FreeTypeFontAtlas::FontDetails* mFontData;
};
