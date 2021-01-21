#include "Scene.h"

#include "ErrorHandling.h"

Scene::Scene(const Movie* movie)
	: mMovie(movie)
{
}

void Scene::setup(ScenePhysicsState* state)
{
	state->setup();
	doSetup(state);
}
