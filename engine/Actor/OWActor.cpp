#include "OWActor.h"
#include "../Core/Scene.h"
#include "../Core/CollisionSystem.h"
#include "../Renderers/InstanceRenderer.h"
#include "../Renderers/MeshRenderer.h"

OWActor::OWActor(Scene* _scene, const std::string& _name, OWActor* _hostActor)
	: mScene(_scene), mName(_name), mHostActor(_hostActor)
{
	_scene->addActor(this);
}

void OWActorDiscrete::doRender(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4 model,
	const glm::vec3& cameraPos) 
{
	for (auto& elm: mElements)
	{
		std::vector<glm::mat4> positions;
		positions.push_back(model * elm.trans->modelMatrix());
		elm.rend->render(positions, proj, view, cameraPos);
	}
}

void OWActorDiscrete::doSetup()
{
	AABB b;
	for (int i = 0; i < mElements.size(); i++)
	{
		DiscreteEntity& elm = mElements[i];
		elm.mesh->setup();
		AABB b1;
		elm.rend->setup(elm.mesh->renderData(b1));
		elm.coll->points(b1);
		if (elm.coll->collisionType() != OWCollider::CollisionType::Permeable)
			CollisionSystem::addCollider(elm.coll, this, i);
		b = b | b1;
	}
	bounds(b);
}

size_t OWActorDiscrete::addComponents(const DiscreteEntity& newElement)
{
	mElements.push_back(newElement);
	DiscreteEntity& elm = mElements.back();
	if (elm.coll == nullptr)
		throw NMSLogicException("OWActorDiscrete [" + name() + "] has no OWCollider. Cannot recover.");
	if (elm.phys == nullptr)
		elm.phys = new OWPhysics(); // Physics may not be used.
	if (elm.mesh == nullptr)
		throw NMSLogicException("OWActorDiscrete [" + name() + "] has no OWMeshComponentBase. Cannot recover.");
	if (elm.mesh->actor() == nullptr)
		elm.mesh->actor(this);
	if (elm.rend == nullptr)
		throw NMSLogicException("OWActorDiscrete [" + name() + "] has no OWRenderer. Cannot recover.");
	if (elm.trans == nullptr)
		elm.trans = new OWTransform(transform()); // Often just need default Transform initially
	elm.trans->hostTransform(transform());
	if (elm.sound == nullptr)
		elm.sound = new OWSoundComponent(); // Not all Actors have sound
	size_t retval = mElements.size() - 1;
	elm.coll->componentIndex(retval);
	return retval;
}

size_t OWActorNCom1Ren::addComponents(const NCom1RenElement& newElement)
{
	mElements.push_back(newElement);
	NCom1RenElement& elm = mElements.back();
	if (elm.coll == nullptr)
		throw NMSLogicException("OWActorNCom1Ren [" + name() + "] has no OWCollider. Cannot recover.");
	if (elm.phys == nullptr)
		elm.phys = new OWPhysics(); // Physics may not be used.
	if (elm.mesh == nullptr)
		throw NMSLogicException("OWActorNCom1Ren [" + name() + "] has no OWMeshComponentBase. Cannot recover.");
	if (elm.mesh->actor() == nullptr)
		elm.mesh->actor(this);
	if (elm.trans == nullptr)
		elm.trans = new OWTransform(transform()); // Often just need default Transform
	elm.trans->hostTransform(transform());
	if (elm.sound == nullptr)
		elm.sound = new OWSoundComponent(); // Not all Actors have sound
	size_t retval = mElements.size() - 1;
	elm.coll->componentIndex(retval);
	return retval;
}

void OWActorNCom1Ren::doSetup()
{
	// Each element of the SSO is position (glm::vec4) + colour (glm::vec4).
	//unsigned int ssoSize = 2 * sizeof(glm::vec4);
	unsigned int ssoSize = 1 * sizeof(glm::vec4);

	// mRenderer relies on the size not being changed
	mSSBO.resize(ssoSize * mElements.size());

	OWRenderData rd;
	AABB b;
	for (int i = 0; i < mElements.size(); i++)
	{
		NCom1RenElement& elm = mElements[i];
		elm.mesh->setup();
		AABB b1, b2, b3;
		OWRenderData rd_elm = elm.mesh->renderData(b1);
		rd.add(rd_elm);
		AABB b123 = b1 | b2 | b3;
		elm.coll->points(b123);
		if (elm.coll->collisionType() != OWCollider::CollisionType::Permeable)
		{
			CollisionSystem::addCollider(elm.coll, this, i);
		}
		b = b | b123;
		const glm::vec3& p = elm.trans->worldPosition();
		size_t offset = i * ssoSize;
		mSSBO[offset + 0] = p.x;
		mSSBO[offset + 1] = p.y;
		mSSBO[offset + 2] = p.z;
		mSSBO[offset + 3] = 0;
		/*
		mSSBO[offset + 4] = elm.colour.x;
		mSSBO[offset + 5] = elm.colour.y;
		mSSBO[offset + 6] = elm.colour.z;
		mSSBO[offset + 7] = elm.colour.w;
		*/
	}
	OWRenderer::SSBO ssbo(mSSBO.data(), mSSBO.size());
	mRenderer->ssbo(ssbo);
	mRenderer->setup(rd);
	bounds(b);
}
void OWActorNCom1Ren::doRender(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4 model,
	const glm::vec3& cameraPos) 
{
	std::vector<glm::mat4> positions;
	for (auto& elm : mElements)
	{
		const glm::mat4 m = model * elm.trans->modelMatrix();
		positions.push_back(m);
	}
	mRenderer->render(positions, proj, view, cameraPos);
}

size_t OWActorMutableParticle::addComponents(const MutableParticleElement& newElement)
{
	mElements.push_back(newElement);
	MutableParticleElement& elm = mElements.back();
	if (elm.coll == nullptr)
		throw NMSLogicException("OWActorMutableParticle [" + name() + "] has no OWCollider. Cannot recover.");
	if (elm.trans == nullptr)
		throw NMSLogicException("OWActorMutableParticle [" + name() + "] has no OWTransform. Cannot recover.");
	else
		elm.trans->hostTransform(transform());
	if (elm.phys == nullptr)
		elm.phys = new OWPhysics(); // Physics may not be used.
	size_t retval = mElements.size() - 1;
	elm.coll->componentIndex(retval);
	return retval;
}

void OWActorMutableParticle::renderer(OWRenderer* newValue)
{
	mRenderer = newValue;
}

void OWActorMutableParticle::sound(OWSoundComponent* newValue)
{
	mSound = newValue;
}

void OWActorMutableParticle::doSetup()
{
	AABB b = AABB(0);
	OWRenderData rd = mMeshTemplate->renderData(b);
	for (int i = 0; i < mElements.size(); i++)
	{
		MutableParticleElement& elm = mElements[i];
		AABB b_moved = b;
		b_moved.move(elm.trans->worldPosition());
		elm.coll->points(b_moved);
		if (elm.coll->collisionType() != OWCollider::CollisionType::Permeable)
		{
			CollisionSystem::addCollider(elm.coll, this, i);
		}
		b = b | b_moved;;
	}
	mRenderer->setup(rd);
	bounds(b);
}

void OWActorMutableParticle::doRender(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4 model,
	const glm::vec3& cameraPos)
{
	std::vector<glm::mat4> positions;
	for (auto& elm : mElements)
	{
		const glm::mat4 m = model * elm.trans->modelMatrix();
		positions.push_back(m);
	}
	mRenderer->render(positions, proj, view, cameraPos);
}

OWActorImmutableParticle::OWActorImmutableParticle(Scene* _scene, const std::string& _name, OWActor* _hostActor)
: OWActor(_scene, _name, _hostActor) 
{

}

void OWActorImmutableParticle::instanceMesh(const InstanceData& _data, std::string& _name)
{
	//mMeshTemplate = new OWMeshComponent(this, _name);
	throw NMSLogicException("OWActorImmutableParticle [" + name() + "] incomplete. Cannot recover.");
	//mMeshTemplate->add(_data);
}

void OWActorImmutableParticle::renderer(OWInstanceRenderer* newValue)
{
	mRenderer = newValue;
}

void OWActorImmutableParticle::sound(OWSoundComponent* newValue)
{
	mSound = newValue;
}

void OWActorImmutableParticle::doSetup()
{
	AABB b;
	const OWRenderData rd = mMeshTemplate->renderData(b);
	mRenderer->setup(rd);
	bounds(b);
}

void OWActorImmutableParticle::doRender(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4 model,
	const glm::vec3& cameraPos)
{
	std::vector<glm::mat4> positions;
	mRenderer->render(positions, proj, view, cameraPos);
}
