#pragma once
#include <vector>

#include <Actor/OWActor.h>
#include <Geometry/GeometricShapes.h>

class TextComponent;

struct OWENGINE_API OWThreeDAxisData
{
	glm::vec4 axisColour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN);
	std::string axisColourName = "colour";
	glm::vec4 labelColour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLUE);
	std::string axisName;
	std::string font = std::string("arial.ttf");
	AABB bounds = AABB(glm::vec3(-100, -100, -100), glm::vec3(100, 100, 100));
};

class OWENGINE_API ThreeDAxis: public OWActorSingle
{
private:
	OWThreeDAxisData mData;
protected:
public:
	ThreeDAxis(Scene* _scene, const std::string& _name, const OWThreeDAxisData& _data = OWThreeDAxisData());
	void doSetup() override;
private:
	OWActorSingle::SingleSceneElement createText(const glm::vec3& pos, const std::string& s, unsigned int refPos, AABB& b);
};
