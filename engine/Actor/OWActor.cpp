#include "OWActor.h"
#include "../Core/Scene.h"
#include "../Core/CollisionSystem.h"

OWActor::OWActor(Scene* _scene, const std::string& _name, OWActor* _hostActor)
	: mScene(_scene), mName(_name), mHostActor(_hostActor)
{
	_scene->addActor(this);
}

void OWActorSingle::doRender(const glm::mat4& proj,
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
		sse.m->setup();
		AABB b1, b2;
		sse.r->setup(sse.m->simpleMesh(b1), sse.m->complexMesh(b2));
		sse.c->points(b1 | b2);
		if (sse.c->collisionType() != OWCollider::CollisionType::Permeable)
			CollisionSystem::addCollider(sse.c, this, i);
	}
}

size_t OWActorSingle::addComponents(const SingleSceneElement& newElement)
{
	mElements.push_back(newElement);
	SingleSceneElement& sse = mElements.back();
	if (sse.c == nullptr)
		throw NMSLogicException("OWActorSingle [" + name() + "] has no Collider. Cannot recover.");
	if (sse.p == nullptr)
		sse.p = new OWPhysics(); // Physics may not be used.
	if (sse.m == nullptr)
		throw NMSLogicException("OWActorSingle [" + name() + "] has no OWMeshComponentBase. Cannot recover.");
	ensureActor(sse.m);
	if (sse.r == nullptr)
		throw NMSLogicException("OWActorSingle [" + name() + "] has no OWRenderer. Cannot recover.");
	if (sse.t == nullptr)
		sse.t = new OWTransform(transform()); // Often just need default Transform
	sse.t->hostingTransform(transform());
	if (sse.s == nullptr)
		sse.s = new OWSoundComponent(); // Not all Actors have sound
	return mElements.size() - 1;
}

void OWActorMulti::addComponents(const MultiSceneElement& newElement)
{
	mElements.push_back(newElement);
	MultiSceneElement& mse = mElements.back();
	if (mse.c == nullptr)
		throw NMSLogicException("OWActorMulti [" + name() + "] has no Collider. Cannot recover.");
	if (mse.p == nullptr)
		mse.p = new OWPhysics(); // Physics may not be used.
	if (mse.t == nullptr)
		mse.t = new OWTransform(transform()); // Often just need default Transform
	mse.t->hostingTransform(transform());
	mse.c->componentIndex(mElements.size()-1);
}

void OWActorMulti::doSetup()
{
	mMeshComponentTemplate->setup();
	AABB b, b1, b2;
	mRenderer->setup(mMeshComponentTemplate->simpleMesh(b1),
		mMeshComponentTemplate->complexMesh(b2));
	b = b1 | b2;
	for (int i = 0; i < mElements.size(); i++)
	{
		MultiSceneElement& mse = mElements[i];
		mse.c->points(b);
		if (mse.c->collisionType() != OWCollider::CollisionType::Permeable)
		{
			CollisionSystem::addCollider(mse.c, this, i);
		}
	}
}

void OWActorMulti::doRender(const glm::mat4& proj,
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

