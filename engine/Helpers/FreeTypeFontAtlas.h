#pragma once
#include "../OWEngine/OWEngine.h"

#include <string>
#include <map>
#include <vector>
#include <filesystem>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "Texture.h"
/*
	Caches loaded FreeType FontAtlas textures. class FreeTypeFontAtlas::FontDetails is
	returned to objects that implement rendered text
*/
struct FT_FaceRec_;
typedef struct FT_FaceRec_*  FT_Face;

class OWENGINE_API FreeTypeFontAtlas
{
public:
	class OWENGINE_API FontDetails
	{
	public:
		FontDetails() {}
		FontDetails(FT_Face face, unsigned int maxRowWidth, int fontHeight);
		~FontDetails();
		float width() const { return static_cast<float>(mWidth); }
		float height() const { return static_cast<float>(mHeight); }
		Texture texture() const { return mTexture; }
		std::vector<glm::vec4> createText(const std::string& text, 
			float sx, float sy) const;

		static glm::vec2 pleasingSpacing(
			int fontHeight, float aspectRatio = 1.0f);
		static void pleasingSpacing(int fontHeight, float x, float y);
	private:
#pragma warning( push )
#pragma warning( disable : 4251 )
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

		unsigned int mWidth = 0; // width of texture in pixel
		unsigned int mHeight = 0; // height of texture in pixels
		static std::map<int, glm::vec2> mNiceFontSpacings;
		std::map<uint8_t, Character> mFontMap;
		Texture mTexture;
		Texture createGlyphBitmap(FT_Face& face, unsigned int maxWidth);
		void calcTextureSize(FT_Face& face, 
				unsigned int& width, unsigned int& height,
				unsigned int maxRowWidth, int fontHeight) const;
	};
	const FontDetails& loadFont(
				const std::filesystem::path& path,
				int fontHeight);
#pragma warning( pop )
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	struct LoadedFace
	{
		LoadedFace(const std::filesystem::path& path, 
					int fontHeight);
		LoadedFace():face(nullptr), maxRowWidth(0){}
		FT_FaceRec_* face;
		std::map<int, FontDetails> fontDimensions;
		unsigned int maxRowWidth;
		void debugData(std::ostream& out, unsigned int fontHeight);
	};
	std::map<std::filesystem::path, LoadedFace> mFonts;
#pragma warning( pop )
};