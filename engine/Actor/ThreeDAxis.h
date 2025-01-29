#pragma once
#include <vector>

#include <Actor/OWActor.h>
#include <Geometry/GeometricShapes.h>

class TextComponent;

struct OWENGINE_API OWThreeDAxisDataImp
{
	glm::vec4 colour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED);
	std::string axisName;
	std::string font = std::string("arial.ttf");
	AABB bounds = AABB(glm::vec3(-100, -100, -100), glm::vec3(100, 100, 100));
	AABB axisSize;
	AABB world;
	bool useGlobalBounds = false;
};

struct OWENGINE_API OWThreeDAxisData : public OWActorData
{
	OWThreeDAxisDataImp axisData;
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
	void doInit() override;
	const OWThreeDAxisData* constData() const
	{
		return static_cast<const OWThreeDAxisData*>(script()->data());
	}
private:
	TextComponent* createText(const glm::vec3& pos, const std::string& s, unsigned int refPos, AABB& b);
	void createAxisData(const AABB& w);
};
