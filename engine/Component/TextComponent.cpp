#include "TextComponent.h"

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderers/TextRendererDynamic.h"
#include "../Renderers/TextRendererStatic.h"
#include "../Renderers/VAOBuffer.h"
#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"

TextComponent::TextComponent(OWActor* _owner, TextComponentData* _data)
	: OWSceneComponent(_owner, _data)
{
}

void TextComponent::doInit()
{
	TextData* jfw = textData();
	name("Text:" + textData()->text);

	// If both left and right then remove them
	if (((textData()->referencePos & TextData::PositionType::Left) &&
		(textData()->referencePos & TextData::PositionType::Right)))
	{
		textData()->referencePos = TextData::PositionType(textData()->referencePos & 0xC);
	}

	// If both Top and Bottom then remove them
	if (((textData()->referencePos & TextData::PositionType::Top) &&
		(textData()->referencePos & TextData::PositionType::Bottom)))
	{
		textData()->referencePos = TextData::PositionType(textData()->referencePos & 0x3);
	}
	TextRenderer* ren;
	if (textData()->tdt == TextData::TextDisplayType::Dynamic)
		ren = new TextRendererDynamic();
	else
		ren = new TextRendererStatic();
	ren->setup(this, glm::vec3(0));
	data()->boundingBox = ren->bounds();
	addRenderer(ren);
}
