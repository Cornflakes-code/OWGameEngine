#pragma once
#include <vector>
#include <string>

#include "../Core/GLApplication.h"
#include "../Helpers/CommonUtils.h"
#include "../Helpers/ResourceFactory.h"
#include "../Helpers/ResourceSource.h"

#include "SimpleVertexSource.h"
#include "../Helpers/TextBillboard.h"


/*
	Draws a fixed sized immutable billboard text
*/
class BBTextFixedSize: public TextBillboard,
				public SimpleVertexSource // So SimpleVertexRender can draw this
{
	const FreeTypeFontAtlas::FontDetails* mFontData;
	glm::vec4 mColor;
	Shader* mShader;
	AABB mBounds;
	mutable float mAspectRatio = 1.0f;
	std::vector<glm::vec4> mVertices;
	void resizeCallback(GLApplication::WindowResizeType resizeType, glm::ivec2 dimensions);
	AABB findBounds() const;
public:
	BBTextFixedSize(const std::string& fontFileName, int fontHeight);
	~BBTextFixedSize();

	void text(const std::string& text, float sx, float sy);
	void color(const glm::vec4& newValue) { mColor = newValue; }

	GLuint texture() const override { return mFontData->texture(); }
	const Shader* shader() const override { return mShader; }
	std::string verticeLocation() const override { return "coord"; }
	std::vector<glm::vec4> vertices() const override { return mVertices; }
	void doRender(const glm::mat4& proj, const glm::mat4& view,
				  const glm::mat4& model) const  override;
};
