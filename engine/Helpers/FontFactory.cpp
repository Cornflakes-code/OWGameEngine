#include "FontFactory.h"

#include <iostream>
#include <fstream>

#include "../Core/ResourcePathFactory.h"
#include "../Core/ErrorHandling.h"
#include "../Core/LogStream.h"

FreeTypeFontAtlas FontFactory::mFreeType;
std::vector<FontFactory::FreeTypeFont> FontFactory::mFreeTypes;

const FreeTypeFontAtlas::FontDetails* FontFactory::loadFreeTypeFont(
	const std::string& fileName, unsigned int fontHeight)
{
	std::experimental::filesystem::path p
		= ResourcePathFactory().appendPath(fileName, 
					ResourcePathFactory::ResourceType::Font);
	return mFreeType.loadFont(p, fontHeight);
}

