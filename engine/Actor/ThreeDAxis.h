#pragma once
#include <vector>

#include <Actor/OWActor.h>
#include <Geometry/GeometricShapes.h>

class TextData;
class OWENGINE_API ThreeDAxis : public OWActor
{
private:
public:
	ThreeDAxis(Scene* _owner, const glm::vec3& _position);
	void createAxisData(const AABB& w);
private:
	TextData* createText(const glm::vec3& pos, const std::string& s, unsigned int refPos, AABB& b);
};
