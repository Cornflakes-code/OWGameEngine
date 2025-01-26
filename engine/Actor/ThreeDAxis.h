#pragma once
#include <vector>

#include <Actor/OWActor.h>
#include <Geometry/GeometricShapes.h>

class TextComponent;

struct OWENGINE_API OWThreeDAxisData: public OWActorData
{
	glm::vec4 Colour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED);
	std::string AxisName;
	std::string Font = std::string("arial.ttf");
	AABB Bounds = AABB(glm::vec3(-100, -100, -100), glm::vec3(100, 100, 100));
	AABB axisSize;
	bool useGlobalBounds = false;
};

class OWENGINE_API OWThreeDAxisScript: public OWActorScript
{
public:
	OWThreeDAxisScript(OWThreeDAxisData* _data)
		: OWActorScript(_data) {}
};

class OWENGINE_API ThreeDAxis: public OWActor
{
private:
protected:
	OWThreeDAxisData* data() 
	{
		return static_cast<OWThreeDAxisData*>(script()->data());
	}
public:
	ThreeDAxis(Scene* _scene, OWThreeDAxisScript* _script);
	const OWThreeDAxisData* constData() const
	{
		return static_cast<const OWThreeDAxisData*>(script()->data());
	}
private:
	TextComponent* createText(const glm::vec3& pos, const std::string& s, unsigned int refPos, AABB& b);
	void createAxisData(const AABB& w);
};
