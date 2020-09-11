#pragma once
#include <Core/Scene.h>
#include <Renderables/BoundingBox.h>

struct NMSWorldPhysicsState: public ScenePhysicsState
{
	NMSWorldPhysicsState(const Scene* owner)
		: ScenePhysicsState(owner) {}
};

/*
	Note that "quit" and "previous" are already used for scene names.
*/
class NMSScene: public Scene
{
	static AABB mWorld;
public:
	static std::string splashSceneName() { return "Splash"; }
	static std::string mainSceneName() { return "Main"; }
	static std::string endSceneName() { return "TheEnd"; }
	NMSScene(const Movie* movie);
	static const AABB& world() { return mWorld; }
private:
};