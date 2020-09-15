#pragma once

#include <string>

#include "../Helpers/ResourceFactory.h"
#include "../Helpers/ResourceSource.h"

class RenderableText: public ResourceSource
{
public:
	RenderableText(const std::string& fontFileName, int fontHeight);
	void text(const std::string& text, float sx, float sy);
private:
	std::string mText;
	float mSx;
	float mSy;
	const FreeTypeFontAtlas::FontDetails* mFontDetails;
};