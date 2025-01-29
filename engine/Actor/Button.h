#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../Actor/OWActor.h"

class TextComponent;

struct OWENGINE_API ButtonData: public OWActorData
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

class OWENGINE_API ButtonScript: public OWActorScript
{
protected:
public:
	ButtonScript(ButtonData* _data)
		: OWActorScript(_data) 
	{}
};

class OWButton : public OWActor
{
	virtual ButtonData* data()
	{
		return static_cast<ButtonData*>(script()->data());
	}
public:
	OWButton(Scene* _owner, ButtonScript* _data);
	virtual const ButtonData* data() const
	{
		return static_cast<const ButtonData*>(script()->data());
	}
	void setup(const ButtonData& data, const glm::vec3& position);
	void textures();
	void text();
protected:
private:
};