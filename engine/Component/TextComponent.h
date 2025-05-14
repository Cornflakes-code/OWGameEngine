#pragma once
#include <vector>

#include <glm/glm.hpp>

#include <Component/Component.h>
#include <Core/CommonUtils.h>
#include "../Renderers/RenderTypes.h"

struct OWENGINE_API OWTextComponentData
{
	enum PositionType : unsigned int
	{
		Center = 0x0,
		Left = 0x1,
		Right = 0x2,
		Top = 0x4,
		Bottom = 0x8
	};
	std::string text;
	std::string fontName = "arial.ttf";
	glm::vec4 colour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLACK);
	glm::vec2 fontSpacing = { 0.0625, 0.125 };
	int fontHeight = 12;
	glm::vec2 magicTextScaleFactor = { 1.0f, 1.0f };
	unsigned int referencePos = PositionType(PositionType::Center & 0xC);
	OWRenderTypes::DrawType tdt = OWRenderTypes::DrawType::TwoDDynamic;
};

class Texture;
class OWENGINE_API OWTextComponent: public OWMeshComponentBase
{
	OWTextComponentData mData;
public:
	OWTextComponent(OWActor* _owner, const std::string& _name,
		const OWTextComponentData& _data);
	OWTextComponent(OWActor* _owner, const std::string& _name,
		const std::string& textFileName);
	void doSetup() override;
	const OWRenderData renderData(AABB& bounds) override;
	static OWRenderTypes::ShaderMutator shaderMutator(OWRenderTypes::DrawType _drawType);
private:
};
