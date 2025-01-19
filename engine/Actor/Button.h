#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../Actor/OWActor.h"

class TextData;

struct ButtonData
{
	enum ClickState
	{
		MouseInside,
		MouseClicked
	};

	std::vector<glm::vec3> mButtonShape;
	TextData* mText = nullptr;
	bool intersectArea(const glm::vec3& pt);
	ClickState mClickState;
};

class OWButton : public OWActor
{
	ButtonData mData;
public:
	OWButton(Scene* _owner, const glm::vec3& _position, const std::string& _displayText);
	void setup(const ButtonData& data, const glm::vec3& position);
	void textures();
	void text();
protected:
private:
};