#pragma once

#include <Core/Scene.h>

#include <Core/BoundingBox.h>

struct NMSWorldPhysicsState: public ScenePhysicsState
{
	NMSWorldPhysicsState(const Scene* owner)
		: ScenePhysicsState(owner) {}
};

/*
	Intermediate Base class for implementation of a Scene for the NMS game. 
	Will be moved out of the engine to a different repo
*/
class NMSScene: public Scene
{
	static AABBV3 mWorld;
public:
	NMSScene(const Movie* movie);
	static const AABBV3& world() { return mWorld; }
private:
};