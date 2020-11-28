#include "FontFactory.h"

#include <iostream>
#include <fstream>

#include "ResourceFactory.h"
#include "ErrorHandling.h"
#include "LogStream.h"

FreeTypeFontAtlas FontFactory::mFreeType;
std::vector<FontFactory::FreeTypeFont> FontFactory::mFreeTypes;

const FreeTypeFontAtlas::FontDetails* FontFactory::loadFreeTypeFont(
	const std::string& fileName, unsigned int fontHeight)
{
	std::experimental::filesystem::path p
		= ResourceFactory().appendPath(fileName, ResourceFactory::ResourceType::Font);
	return mFreeType.loadFont(p, fontHeight);
}

