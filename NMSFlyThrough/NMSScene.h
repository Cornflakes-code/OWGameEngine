#pragma once

#include <Core/Scene.h>

#include <Geometry/BoundingBox.h>

/*
	Intermediate Base class for implementation of a Scene for the NMS game. 
	Will be moved out of the engine to a different repo
*/
class NMSScene: public Scene
{
	static AABB mWorld;
public:
	NMSScene(const Movie* movie);
	static const AABB& world() { return mWorld; }
private:
};