#include "Scene.h"

#include "ErrorHandling.h"
#include "../Actor/OWActor.h"
#include "../Core/CollisionSystem.h"

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


void Scene::addCollider(OWCollider* coll, OWActor* a, int componentId)
{
	CollisionSystem::addCollider(coll, a, componentId);
}

void Scene::setup()
{
	doSetupScene();
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

void Scene::timeStep(std::string& nextScene, OWUtils::Time::duration fixedStep)
{
	traverseSceneGraph([](OWActor* a)
		{
			a->preTick();
		}
	);
	CollisionSystem::preTick();

	float timeStep = std::chrono::duration<float>(fixedStep).count();
	traverseSceneGraph([timeStep](OWActor* a)
		{
			a->tick(timeStep);
		}
	);
	CollisionSystem::postTick();
	traverseSceneGraph([timeStep](OWActor* a)
		{
			a->postTick();
		}
	);
}

void Scene::preRender(float totalTime, float alpha, float fixedTimeStep)
{
	traverseSceneGraph([](OWActor* a)
		{
			a->preRender();
		}
	);
	traverseSceneGraph([totalTime, alpha, fixedTimeStep](OWActor* a)
		{
			a->interpolatePhysics(totalTime, alpha, fixedTimeStep);
		});
}

void Scene::render(const glm::mat4& proj, const glm::mat4& view,
	const glm::vec3& cameraPos)
{
	doRenderScene(proj, view, cameraPos);

	auto rend = [proj, view, cameraPos](OWActor* a)
		{
			a->render(proj, view, cameraPos);
		};
	traverseSceneGraph(rend);
}
