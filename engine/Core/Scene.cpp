#include "Scene.h"

#include "ErrorHandling.h"

Scene::Scene(const Movie* movie)
	: mMovie(movie)
{
}

void Scene::addActor(OLDActor* a)
{
	mRootNode.push_back(a);
}

void Scene::traverseSceneGraph(OWActorCallbackType cb) const
{
	for (OLDActor* a : mRootNode)
	{
		cb(a);
	}
}

void Scene::setup(ScenePhysicsState* state)
{
	state->setup();
	doSetup(state);
}
