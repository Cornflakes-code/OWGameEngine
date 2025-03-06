#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../Actor/OWActor.h"

class TextComponent;

struct OWENGINE_API ButtonData
{
	enum ClickState
	{
		MouseInside,
		MouseClicked
	};

	std::vector<glm::vec3> mButtonShape;
	TextComponent* mText = nullptr;
	bool intersectArea(const glm::vec3& pt);
	ClickState mClickState;
};

class OWENGINE_API ButtonScript
{
protected:
public:
	ButtonScript(ButtonData* _data)
	{}
};

class OWButton: public OWActor
{
public:
	OWButton(Scene* _scene, const std::string& _name);
	void doSetup() override;
	//void textures();
	//void text();
protected:
private:
};