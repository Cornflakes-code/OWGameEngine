#pragma once
#include <string>

#include "../OWEngine/OWEngine.h"

#include "FreeTypeFontAtlas.h"

class OWENGINE_API FontFactory
{
public:
	const FreeTypeFontAtlas::FontDetails* loadFreeTypeFont(
		const std::string&, unsigned int fontHeight);
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	struct FreeTypeFont
	{
		FT_Face face;
		std::string fileName;
		int fontHeight;
	};
	static FreeTypeFontAtlas mFreeType;
	static std::vector<FreeTypeFont> mFreeTypes;
#pragma warning( pop )
};