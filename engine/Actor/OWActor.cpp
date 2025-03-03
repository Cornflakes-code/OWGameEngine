#include "OWActor.h"
#include "../Core/Scene.h"
#include "../Helpers/Collider.h"
#include "../Renderers/RendererBase.h"
#include "CollisionSystem.h"

OWActor::OWActor(Scene* _scene, const std::string& _name)
	: mScene(_scene), mName(_name)
{
	_scene->addActor(this);
}

void OWActorSingle::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4 model,
	const glm::vec3& cameraPos) 
{
	for (auto& sse : mElements)
	{
		std::vector<glm::mat4> positions;
		positions.push_back(model * sse.t->modelMatrix());
		sse.r->render(positions, proj, view, cameraPos);
	}
}

void OWActorSingle::doSetup()
{
	for (int i = 0; i < mElements.size(); i++)
	{
		SingleSceneElement& sse = mElements[i];
		sse.m->doSetup();
		AABB bounds = sse.r->setup(sse.m->simpleMesh(), sse.m->complexMesh());
		sse.c->points(bounds);
		if (sse.c->collisionType() != OWCollider::CollisionType::Permeable)
			CollisionSystem::addCollider(sse.c, this, i);
	}
}

void OWActorSingle::setComponent(const SingleSceneElement& sse)
{
}

void OWActorMulti::addElement(const MultiSceneElement& toAdd)
{
	if (toAdd.t->actor() == nullptr)
		toAdd.t->actor(this);

	mElements.push_back(toAdd);
	unsigned int ndx = mElements.size() - 1;
	MultiSceneElement& mse = mElements.back();
	mse.c->componentIndex(ndx);
	glm::vec3 dummy = glm::vec3(0);
	mse.c->points(dummy, dummy, dummy); // need to do this once the size of the mesh is known;
}

void OWActorMulti::doSetup()
{
	mRenderer->doSetup(mMeshComponentTemplate->simpleMesh(),
		mMeshComponentTemplate->complexMesh());
}

void OWActorMulti::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4 model,
	const glm::vec3& cameraPos) 
{
	std::vector<glm::mat4> positions;
	for (auto& mse : mElements)
	{
		const glm::mat4 m = model * mse.t->modelMatrix();
		positions.push_back(m);
	}
	mRenderer->render(positions, proj, view, cameraPos);
}

void OWActorMulti::physics(OWPhysics* newValue)
{
	mPhysics = newValue;
}

void OWActorMulti::renderer(OWRenderer* newValue)
{
	mRenderer = newValue;
}

void OWActorMulti::sound(OWSoundComponent* newValue)
{
	mSound = newValue;
}

