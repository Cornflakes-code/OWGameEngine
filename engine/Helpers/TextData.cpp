#include "TextData.h"

#include "../Renderers/TextRendererDynamic.h"
#include "../Renderers/TextRendererStatic.h"

void TextData::prepare()
{
	name("Text:" + mText);

	TextRenderer* ren;
	if (mDynamicSize == Dynamic)
		ren = new TextRendererDynamic();
	else
		ren = new TextRendererStatic();
	ren->setup(this, glm::vec3(0));
	bounds(ren->bounds());
	addRenderer(ren);
	readyForRender();
}
