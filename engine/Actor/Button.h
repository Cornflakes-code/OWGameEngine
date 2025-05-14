#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../Actor/OWActor.h"
#include <glm/gtx/transform.hpp>

class TextComponent;

struct OWENGINE_API OWButtonData
{
	enum ClickState
	{
		MouseInside,
		MouseClicked
	};

	//std::vector<glm::vec3> mButtonShape;
	std::string text = "CLICKABLE";
	bool intersectArea(const glm::vec3& pt);
	ClickState mClickState;
	glm::vec4 innerColour = OWUtils::colour(OWUtils::SolidColours::BLUE);
	glm::vec4 outerColour = OWUtils::colour(OWUtils::SolidColours::RED);
	OWTransformData td = OWTransformData();
};

class OWENGINE_API OWButton: public OWActorDiscrete
{
public:
	OWButton(Scene* _scene, const std::string& _name);
	void initialise(const OWButtonData& _data);
	//void textures();
	//void text();
protected:
	OWActorDiscrete::DiscreteEntity makeShape(const std::string& s, const glm::vec4& colour, 
			const glm::vec3& size, OWCollider::CollisionType collType);
private:
	OWButtonData mData;
};