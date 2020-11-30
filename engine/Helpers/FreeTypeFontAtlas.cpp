#include "FreeTypeFontAtlas.h"

#include <limits>
#include <algorithm>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ErrorHandling.h"
#include "LogStream.h"

const FreeTypeFontAtlas::FontDetails* FreeTypeFontAtlas::loadFont(
		const std::experimental::filesystem::path& path, int fontHeight)
{
	auto iter = mFonts.begin();
	while (iter != mFonts.end())
	{
		if (std::experimental::filesystem::equivalent(iter->first, path))
			break;
		++iter;
	}
	if (iter == mFonts.end())
	{
		mFonts[path] = LoadedFace(path, fontHeight);
		iter = mFonts.find(path);
	}

	auto fontHeightDataIter = iter->second.fontDimensions.find(fontHeight);
	if (fontHeightDataIter == iter->second.fontDimensions.end())
	{
		iter->second.fontDimensions[fontHeight]
			= FontDetails(iter->second.face, 
					iter->second.maxRowWidth, fontHeight);
		LogStream log(LogStreamLevel::Info);
		iter->second.debugData(log, fontHeight);
	}
	return &(iter->second.fontDimensions[fontHeight]);
}

FreeTypeFontAtlas::LoadedFace::LoadedFace(
		const std::experimental::filesystem::path& path, int fontHeight)
	: maxRowWidth(512) // Nothing really magical about 512
{
	FT_Library ft;

	if (FT_Init_FreeType(&ft))
	{
		throw NMSException("Error: Could not init FreeType Library");
	}

	// load font as face
	if (FT_New_Face(ft, path.string().c_str(), 0, &face))
	{
		throw NMSException(std::stringstream() << "Error: Font ["
			<< path.string() << "] found but Freetype could not load it.\n");
	}
}

void FreeTypeFontAtlas::LoadedFace::debugData(
			std::ostream& out, unsigned int fontHeight)
{
	out << "For face " << face->family_name
		<< ", font height " << fontHeight << ", generated a "
		<< fontDimensions[fontHeight].width()
		<< " * " << fontDimensions[fontHeight].height()
		<< "(" << fontDimensions[fontHeight].width() *
		fontDimensions[fontHeight].height() / 1024
		<< "kb) texture atlas\n";
}

// Allocate storage for the static
std::map<int, glm::vec2> FreeTypeFontAtlas::FontDetails::mNiceFontSpacings;

FreeTypeFontAtlas::FontDetails::FontDetails(FT_Face face, 
	unsigned int maxRowWidth, int fontHeight)
{
	calcTextureSize(face, mWidth, mHeight, maxRowWidth, fontHeight);
	mTexture = createGlyphBitmap(face, maxRowWidth);
}

FreeTypeFontAtlas::FontDetails::~FontDetails()
{
}


glm::vec2 FreeTypeFontAtlas::FontDetails::pleasingSpacing
		(int fontHeight, float aspectRatio)
{
	auto iter = mNiceFontSpacings.find(fontHeight);
	if (iter == mNiceFontSpacings.end())
		throw NMSException(std::stringstream()
			<< "Font scaling not set for Font height [" 
			<< fontHeight << "]\n");

	glm::vec2 retval = mNiceFontSpacings[fontHeight];
	if (aspectRatio < 1.0f)
		retval.x *= aspectRatio;
	else
		retval.y *= aspectRatio;
	return retval;
}

void FreeTypeFontAtlas::FontDetails::pleasingSpacing(
	int fontHeight, float x, float y)
{
	mNiceFontSpacings[fontHeight] = { x, y };
}

void FreeTypeFontAtlas::FontDetails::calcTextureSize(
			FT_Face& face, 
			unsigned int& width, unsigned int& height,
			unsigned int maxRowWidth, int fontHeight) const
{

	if (FT_Set_Pixel_Sizes(face, 0, fontHeight))
	{
		throw NMSException(std::stringstream() << "FT_Set_Pixel_Sizes failed"
			<< face->style_name << "Font Height [" << fontHeight << "]");
	}

	unsigned int rowWidth = 0;
	unsigned int rowHeight = 0;

	// Find minimum size for a texture holding all visible ASCII characters
	for (uint8_t i = 32; i < 128; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			LogStream(LogStreamLevel::Warning) 
				<< "Loading FT character [" << i << "] failed";
			continue;
		}

		if (rowWidth + face->glyph->bitmap.width + 1 >= maxRowWidth)
		{
			if (rowWidth > width)
				width = rowWidth;
			height += rowHeight;
			rowHeight = 0;
			rowWidth = 0;
		}
		rowWidth += face->glyph->bitmap.width + 1;
		rowHeight = std::max(rowHeight, face->glyph->bitmap.rows);
	}

	if (rowWidth > width)
		width = rowWidth;
	height += rowHeight;
}

Texture FreeTypeFontAtlas::FontDetails::createGlyphBitmap(
					FT_Face& face, unsigned int maxWidth)
{
	// Create a texture that will be used to hold all ASCII glyphs
	// The next step is probably to go to mipmaps. See
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
	Texture texture;
	Texture::InitData initData;
	initData.filter = GL_LINEAR;
	initData.wrap = GL_REPEAT;
	initData.clamp = GL_CLAMP_TO_EDGE;
	texture.init(nullptr, mWidth, mHeight, initData);

	GLenum err = glGetError();
	unsigned int rowHeight = 0;
	GLint yOffset = 0;
	GLint xOffset = 0;

	//glGenerateMipmap(GL_TEXTURE_2D);

	err = glGetError();
	// We require 1 byte alignment when uploading texture data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Paste all glyph bitmaps into the texture.
	FT_GlyphSlot g = face->glyph;

	for (uint8_t i = 32; i < 128; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			LogStream(LogStreamLevel::Info) << "Loading character " 
				<< i << " failed!";
			continue;
		}

		if (xOffset + g->bitmap.width + 1 >= maxWidth)
		{
			yOffset += rowHeight;
			rowHeight = 0;
			xOffset = 0;
		}
		glTexSubImage2D(texture.target(),
			initData.level,
			xOffset, yOffset, // offsets
			g->bitmap.width, g->bitmap.rows, // width, height
			GL_RED, // format
			initData.bitmapType,
			g->bitmap.buffer); // data
		err = glGetError();

		if ((xOffset + g->bitmap.width) > mWidth ||
			(yOffset + g->bitmap.rows) > mHeight)
		{
			err = glGetError();
			if (err != GL_NO_ERROR)
			{
				LogStream(LogStreamLevel::Info) 
					<< "glTexSubImage2D Error: [" << std::hex << err
					<< "]" << std::dec << " index[" << i << "]";
				continue;
			}
		}

		FreeTypeFontAtlas::FontDetails::Character ch;
		ch.advanceX = g->advance.x >> 6;
		ch.advanceY = g->advance.y >> 6;

		ch.bitmapWidth = g->bitmap.width;
		ch.bitmapHeight = g->bitmap.rows;

		ch.bitmapLeft = g->bitmap_left;
		ch.bitmapTop = g->bitmap_top;

		ch.offsetX = static_cast<float>(xOffset);
		ch.offsetY = static_cast<float>(yOffset);

		mFontMap.insert({ i, ch });
		rowHeight = std::max(rowHeight, g->bitmap.rows);
		xOffset += g->bitmap.width + 1;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

std::vector<glm::vec4> FreeTypeFontAtlas::FontDetails::createText(
					const std::string& text, float sx, float sy) const
{
	// See
	// https://stackoverflow.com/questions/20271331/opengl-font-rendering-using-freetype2
	float x = 0.0f;
	float y = 0.0f;
	std::vector<glm::vec4> retval;
	for (int i = 0; i < text.size(); i++)
	{
		// Calculate the vertex and texture coordinates
		const FontDetails::Character& ch = mFontMap.at(text[i]);
		float x2 = x + ch.bitmapLeft * sx;
		float y2 = -y - (ch.bitmapTop) * sy;
		float w = ch.bitmapWidth * sx;
		float h = ch.bitmapHeight * sy;
		float offY = ch.offsetY / height();
		float offX = ch.offsetX / width();

		// Advance the cursor to the start of the next character
		x += ch.advanceX * sx;
		y += ch.advanceY * sy;

		// Skip glyphs that have no pixels
		if (!w || !h)
			continue;

		retval.push_back({
			x2,
			-y2,
			offX,
			offY });
		retval.push_back({
			x2 + w,
			-y2,
			offX + ch.bitmapWidth / width(),
			offY });
		retval.push_back({
			x2,
			-y2 - h,
			offX,
			offY + ch.bitmapHeight / height() });

		retval.push_back({
			x2 + w,
			-y2,
			offX + ch.bitmapWidth / width(),
			offY });
		retval.push_back({
			x2,
			-y2 - h,
			offX,
			offY + ch.bitmapHeight / height() });
		retval.push_back({
			x2 + w,
			-y2 - h,
			offX + ch.bitmapWidth / width(),
			offY + ch.bitmapHeight / height() });
	}
	return retval;
}
