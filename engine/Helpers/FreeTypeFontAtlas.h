#pragma once
#include <string>
#include <map>
#include <vector>
#include <experimental/filesystem>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
/*
	Caches loaded FreeType FontAtlas textures. class FreeTypeFontAtlas::FontDetails is
	returned to objects that implement rendered text
*/
class FreeTypeFontAtlas
{
public:
	class FontDetails
	{
	public:
		FontDetails() {}
		FontDetails(FT_Face face, unsigned int maxRowWidth, int fontHeight);
		~FontDetails();
		float width() const { return static_cast<float>(mWidth); }
		float height() const { return static_cast<float>(mHeight); }
		GLuint texture() const { return mTextureId; }
		std::vector<glm::vec4> createText(const std::string& text, float sx, float sy) const;

		static glm::vec2 pleasingSpacing(int fontHeight, float aspectRatio = 1.0f);
		static void pleasingSpacing(int fontHeight, float x, float y);
	private:
		void calcTextureSize(FT_Face& face, unsigned int maxRowWidth, int fontHeight);

		static std::map<int, glm::vec2> mNiceFontSpacings;

		struct Character
		{
			unsigned int advanceX;	// advance.screenX
			unsigned int advanceY;	// advance.screenY
			unsigned int bitmapWidth;	// bitmap.width;
			unsigned int bitmapHeight;	// bitmap.height;
			int bitmapLeft;	// bitmap_left;
			int bitmapTop;	// bitmap_top;
			float offsetX;	// screenX offset of glyph in texture coordinates
			float offsetY;	// screenY offset of glyph in texture coordinates
		};
		GLuint createGlyphBitmap(FT_Face& face, unsigned int maxWidth);

		unsigned int mWidth = 0; // width of texture in pixel
		unsigned int mHeight = 0; // height of texture in pixels
		GLuint mTextureId = 0;
		std::map<uint8_t, Character> mFONTMap;
	};
	const FontDetails* loadFont(const std::experimental::filesystem::path& path,
							int fontHeight);
private:

	struct LoadedFace
	{
		LoadedFace(const std::experimental::filesystem::path& path, int fontHeight);
		LoadedFace() {}
		FT_Face face;
		unsigned int maxRowWidth;
		std::map<int, FontDetails> fontDimensions;
	};
	std::map<std::experimental::filesystem::path, LoadedFace> mFonts;
};