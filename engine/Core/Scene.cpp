#include "Scene.h"

#include "../Helpers/ErrorHandling.h"

Scene::Scene(const Movie* movie)
	: mMovie(movie)
{
}

void Scene::setup(ScenePhysicsState* state)
{
	doSetup(state);
}
