#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../Actor/OWActor.h"

class TextComponent;

struct OWENGINE_API OWButtonData
{
/*
	enum ClickState
	{
		MouseInside,
		MouseClicked
	};

	std::vector<glm::vec3> mButtonShape;
	TextComponent* mText = nullptr;
	bool intersectArea(const glm::vec3& pt);
	ClickState mClickState;
*/
};

class OWENGINE_API OWButton: public OWActorDiscrete
{
public:
	OWButton(Scene* _scene, const std::string& _name);
	void initialise(const OWButtonData& _data = OWButtonData());
	//void textures();
	//void text();
protected:
	OWActorDiscrete::DiscreteEntity makeShape(const std::string& s, const glm::vec4& colour, const glm::vec3& size);
private:
};