#include "Scene.h"

#include "ErrorHandling.h"
#include "../Actor/OWActor.h"

Scene::Scene(const Movie* movie)
	: mMovie(movie)
{
}

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
	doSetupScene(state);
	auto init = [](OWActor* a)
		{
			a->setup();
		};
	bool moreToGo = true;
	auto testFinished = [&moreToGo](OWActor* a)
		{
			if (!a->setupCompleted())
				moreToGo = true;
		};
	while (moreToGo)
	{
		traverseSceneGraph(init);
		moreToGo = false;
		traverseSceneGraph(testFinished);
	}
}

void Scene::render(const ScenePhysicsState* state,
	const glm::mat4& proj, const glm::mat4& view,
	const glm::vec3& cameraPos)
{
	doRenderScene(state, proj, view, cameraPos);
	glm::mat4 model(1.0);

	auto rend = [proj, view, model, cameraPos](OWActor* a)
		{
			a->render(proj, view, model, cameraPos);
		};
	traverseSceneGraph(rend);
}
