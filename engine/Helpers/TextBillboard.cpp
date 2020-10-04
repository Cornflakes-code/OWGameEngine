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
#include "../Core/GlobalSettings.h"

TextBillboard::TextBillboard(const glm::vec3& initialPosition,
	const std::string& fontFileName, int fontHeight)
	:VertexSource(initialPosition)
{
	mFontData = globals->resourceCache()->loadFreeTypeFont(fontFileName, fontHeight);
}

TextBillboard::~TextBillboard()
{
//	globals->application()->removeWindowResizeListener(this);
}

void TextBillboard::createText(const std::string& text, float sx, float sy)
{
	mVec4 = mFontData->createText(text, sx, sy);
	OWUtils::TextureBlock tb = mFontData->texture();

	// A bit dodgy because we only know this string because of knowledge of ALL 
	// shaders in derived classes.
	tb.name = "textureImageId";

	texture(tb);
}
