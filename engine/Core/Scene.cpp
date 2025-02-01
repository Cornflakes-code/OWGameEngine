#include "Scene.h"

#include "ErrorHandling.h"

Scene::Scene(const Movie* movie)
	: mMovie(movie)
{
}

static std::vector<OWActor*> mRootNode;
void Scene::addActor(OWActor* a)
{
	mRootNode.push_back(a);
}

void Scene::traverseSceneGraph(OWActorCallbackType cb) const
{
	for (OWActor* a : mRootNode)
	{
		cb(a);
	}
}

void Scene::setup(ScenePhysicsState* state)
{
	state->setup();
	doSetup(state);
}
