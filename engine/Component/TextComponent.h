#pragma once
#include <vector>

#include <glm/glm.hpp>

#include <Component/OWSceneComponent.h>
#include <Core/CommonUtils.h>

struct OWENGINE_API TextData
{
	enum PositionType : unsigned int
	{
		Center = 0x0,
		Left = 0x1,
		Right = 0x2,
		Top = 0x4,
		Bottom = 0x8
	};
	enum TextDisplayType
	{
		Dynamic,
		Static
	};
	std::string text;
	std::string fontName = "arial.ttf";
	glm::vec4 colour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLACK);
	glm::vec2 fontSpacing;
	glm::vec2 fontScale = { 1.0,1.0 };
	int fontHeight = 12;
	unsigned int referencePos = PositionType(PositionType::Center & 0xC);
	TextDisplayType tdt = Dynamic;
};

struct OWENGINE_API TextComponentData: public OWSceneComponentData
{
	TextData textData;
};

class OWENGINE_API TextComponent: public OWSceneComponent
{
protected:
	TextData* textData()
	{
		return &(static_cast<TextComponentData*>(OWSceneComponent::data())->textData);
	}
public:
	const TextData* textData() const
	{
		return &(static_cast<const TextComponentData*>(OWSceneComponent::constData())->textData);
	}

	TextComponent(OWActor* _owner)
		: OWSceneComponent(_owner)
	{}
	const TextComponentData* constData() const override
	{
		return static_cast<const TextComponentData*>(OWSceneComponent::constData());
	}

	TextComponent(OWActor* _owner, TextComponentData* _data);

	void init() override;
protected:
private:
	friend class TextRenderer;
};
