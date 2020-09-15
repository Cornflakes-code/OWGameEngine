#include "RenderableText.h"


RenderableText::RenderableText(const std::string& fontFileName, int fontHeight)
{
	mFontDetails = factory()->loadFreeTypeFont(fontFileName, fontHeight);
}

void RenderableText::text(const std::string& text, float sx, float sy)
{
	mText = text,
		mSx = sx;
	mSy = sy;
}
