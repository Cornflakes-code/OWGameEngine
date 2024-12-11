#pragma once
#include <vector>

#include <Core/Actor.h>
#include <Helpers/GeometricShapes.h>

class OWENGINE_API ThreeDAxis : public Actor
{
private:
public:
	ThreeDAxis(Physical* _physical, Actor* _owner): Actor(_physical, _owner) {}
	void createAxisData(const AABB& w);
private:
};
