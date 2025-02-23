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
	int fontHeight = 12;
	unsigned int referencePos = PositionType(PositionType::Center & 0xC);
	TextDisplayType tdt = Dynamic;
};

struct OWENGINE_API TextComponentData: public OLDSceneComponentData
{
	TextData textData;
};

class OWENGINE_API TextComponent: public OLDSceneComponent
{
protected:
	TextData* textData()
	{
		return &(static_cast<TextComponentData*>(OLDSceneComponent::data())->textData);
	}
public:
	const TextData* constTextData() const
	{
		return &(static_cast<const TextComponentData*>(OLDSceneComponent::constData())->textData);
	}

	TextComponent(OLDActor* _owner)
		: OLDSceneComponent(_owner)
	{}
	const TextComponentData* constData() const override
	{
		return static_cast<const TextComponentData*>(OLDSceneComponent::constData());
	}

	TextComponent(OLDActor* _owner, TextComponentData* _data);

	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) override;
	void doInit() override;
protected:
private:
	friend class TextRenderer;
};
