#pragma once
#include <vector>

#include <glm/glm.hpp>

#include <Core/OWSceneComponent.h>
#include <Core/CommonUtils.h>

class OWENGINE_API TextData: public OWSceneComponent
{
public:
	enum PositionType: unsigned int
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

	TextData(OWActor* _owner, const glm::vec3& _position, TextDisplayType tdt)
		: OWSceneComponent(_owner, _position), mDynamicSize(tdt)
	{}

	TextData(OWActor* _owner, const glm::vec3& _position, const std::string& s, TextDisplayType tdt, float sx, float sy)
		: OWSceneComponent(_owner, _position)
		, mText(s)
		, mX(sx)
		, mY(sy)
		, mModified(true)
		, mDynamicSize(tdt)
	{}

	void typeSetDetails(const std::string& s, int height,
		const glm::vec2& _spacing, const glm::vec2& scale,
		const std::string& f = "arial.ttf",
		const glm::vec4& col = { 0.0, 0.0, 0.0, 1.0f })
	{
		text(s);
		font(f, height);
		colour(col);
		spacing(_spacing.x, _spacing.y, scale);
	}
	void font(const std::string& fontFileName, int fontHeight)
	{
		mFontFileName = fontFileName;
		mFontHeight = fontHeight;
		mModified = true;
	}
	void spacing(float sx, float sy, const glm::vec2& scale = glm::vec2(1.0, 1.0), 
				unsigned int referencePos = PositionType::Center)
	{
		mX = sx; mY = sy; mScale = scale;
		// If both left and right then remove them
		if (((referencePos & TextData::PositionType::Left) && 
			(referencePos & TextData::PositionType::Right)))
		{
			referencePos = TextData::PositionType(referencePos & 0xC);
		}

		// If both Top and Bottom then remove them
		if (((referencePos & TextData::PositionType::Top) && 
			(referencePos & TextData::PositionType::Bottom)))
		{
			referencePos = TextData::PositionType(referencePos & 0x3);
		}

		mReferencePos = referencePos;
		mModified = true;
	}
	void text(const std::string& s)
	{
		mText = s; 
		mModified = true;
	}
	void colour(const glm::vec4& colour)
	{
		mColour = colour;
		mModified = true;
	}
	glm::vec2 scale() const { return mScale; }
	void prepare();
protected:
private:
	TextDisplayType mDynamicSize = Dynamic;
	std::string mText;
	std::string mFontFileName = "arial.ttf";
	glm::vec4 mColour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLACK);
	glm::vec2 mScale = glm::vec2({ 1.0, 1.0 });
	unsigned int mReferencePos = PositionType(PositionType::Center & 0xC);
	float mX = 0.0f;
	float mY = 0.0f;
	int mFontHeight = 12;
	bool mModified = false;
	friend class TextRenderer;
};
