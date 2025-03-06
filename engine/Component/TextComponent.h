#pragma once
#include <vector>

#include <glm/glm.hpp>

#include <Component/Component.h>
#include <Core/CommonUtils.h>

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

class OWENGINE_API OWTextComponent : public OWMeshComponentBase
{
	OWTextComponentData mData;
public:
	OWTextComponent(OWActor* _owner, const std::string& _name,
		const OWTextComponentData& _data);
	OWTextComponent(OWActor* _owner, const std::string& _name,
		const std::string& textFileName);
	void doSetup() override;
	virtual const std::vector<OWMeshData> simpleMesh(AABB& bounds) const;
private:
	void prepareMutators();
};
