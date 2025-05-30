#include "Scene.h"

#include <chrono>

#include "ErrorHandling.h"
#include "../Actor/OWActor.h"
#include <Geometry/GeometricShapes.h>
#include <Helpers/Shader.h>
#include <Renderers/MeshRenderer.h>

#include "LogStream.h"

Scene::Scene(const Movie* movie)
	: mMovie(movie), mBoundsActor(this, "Collider Bounds"), mColisionSystem(this)
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

void Scene::addCollider(OWCollider* coll, OWActor* a, OWSize componentId)
{
	mColisionSystem.addCollider(coll, a, componentId);
	mBoundsActor.addCollider(coll, a, componentId);
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

#include <algorithm>
//#define DEBUG_COLLISION
void Scene::timeStep(std::string& nextScene, OWUtils::Time::duration fixedStep)
{
//	mRootNode.erase(std::remove(mRootNode.begin(), mRootNode.end(), 
//		[](const OWActor* x) { return !x->active(); }), mRootNode.end());
	mColisionSystem.sort();

	traverseSceneGraph([](OWActor* a)
		{
			a->preTick();
		}
	);

	float timeStep = std::chrono::duration<float>(fixedStep).count();
	traverseSceneGraph([timeStep](OWActor* a)
		{
			a->tick(timeStep);
		}
	);
#ifdef DEBUG_COLLISION
	OWUtils::Time::time_point startTime = OWUtils::Time::now();
#endif
	mColisionSystem.collide();
#ifdef DEBUG_COLLISION
	OWUtils::Time::duration duration = OWUtils::Time::now() - startTime;
	float frameTimeSecs = std::chrono::duration<float>(duration).count();
	LogStream(LogStreamLevel::ImportantInfo) << "\t        CollisionSystem::postTick()[" << frameTimeSecs << "] > dt ********** \n";
	#endif
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
