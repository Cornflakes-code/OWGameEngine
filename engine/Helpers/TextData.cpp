#include "TextData.h"

#include <Renderers/TextRendererDynamic.h>
#include <Renderers/TextRendererStatic.h>

void TextData::prepare()
{
	mName = "Text: " + mText;

	TextRenderer* ren;
	if (mDynamicSize == Dynamic)
		ren = new TextRendererDynamic();
	else
		ren = new TextRendererStatic();
	ren->setup(this, position());
	bounds(ren->bounds());
	addRenderer(ren);
	readyForRender();
}
