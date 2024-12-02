#pragma once
#include <vector>

#include <Core/Actor.h>
#include <Helpers/GeometricShapes.h>

class OWENGINE_API ThreeDAxis : public Actor
{
private:
public:
	ThreeDAxis(Actor* _owner) : Actor(_owner) {}
	void createAxisData(const AABBV3& w);
private:
};
