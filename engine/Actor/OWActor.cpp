#include "OWActor.h"
#include "../Core/Scene.h"
#include "../Renderers/MeshRenderer.h"

static std::string getActorDesc(OWActor* a)
{
	const OWActor& aref = *a;
	return std::string(typeid(aref).name()) + "[" + a->name() + "]";
}

OWActor::OWActor(Scene* _scene, const std::string& _name, OWActor* _hostActor)
	: mScene(_scene), mName(_name), mHostActor(_hostActor), mScriptor(this)
{
	_scene->addActor(this);
	if (_hostActor != nullptr)
	{
		if (_hostActor->transform() == nullptr)
		{
			throw NMSLogicException("OWActor [" + _hostActor->name()
				+ "] must have a transform before creating sub Actors. Cannot recover.");
		}
		this->transform(new OWTransform(_hostActor->transform()));
	}
	else
	{
		this->transform(new OWTransform(nullptr));
	}
}

void OWActor::setup()
{
	if (!mSetup)
	{
		doSetupActor();
		mSetup = true;
	}
}

/*
void OWActor::callMutators(const OWCollider* coll, const OWMeshComponentBase* mesh,
	const OWPhysics* phys, OWTransform* trans, OWRenderer* rend)
{
	for (auto& cb : mMutatorCallbacks)
	{
		cb(coll, mesh, phys, trans, rend);
	}
}
*/

void OWActor::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::vec3& cameraPos)
{
	if (mSetup && active())
	{
		doRender(proj, view, cameraPos);
	}
}

static void validateCollider(OWActor* a, OWCollider* coll)
{
	if (coll == nullptr)
	{
		throw NMSLogicException(getActorDesc(a) + " has no OWCollider.Cannot recover.");
	}
}

static void validatePhysics(OWActor* a, OWPhysics*& phys)
{
	if (phys == nullptr)
		phys = new OWPhysics(); // Physics is often the default.
}

static void validateMeshComponent(OWActor* a, const OWMeshComponentBase* mesh)
{
	if (mesh == nullptr)
	{
		throw NMSLogicException(getActorDesc(a) + " has no OWMeshComponentBase. Cannot recover.");
	}
	if (mesh->actor() == nullptr)
	{
		throw NMSLogicException(getActorDesc(a) + " OWMeshComponent has no owner. Cannot recover.");
	}
	if (mesh->actor() != a)
	{
		throw NMSLogicException(getActorDesc(a) + " OWMeshComponent has an invalid owner. Cannot recover.");
	}
}

static void validateTransform(OWActor* a, OWTransform*& trans)
{
	if (trans == nullptr)
		trans = new OWTransform(a->transform()); // Often just need default Transform
	if (trans->hostTransform() == nullptr)
		trans->hostTransform(a->transform());
	if (trans->hostTransform() != a->transform())
	{
		throw NMSLogicException(getActorDesc(a) + " transform mismatch. Cannot recover.");
	}
	if (a->hostActor() != nullptr)
	{
		const OWTransform* parT1 = a->hostActor()->transform();
		if (a->transform() == nullptr)
		{
			throw NMSLogicException(getActorDesc(a) + " has a hostActor() but it's transform does not have a host transform. Cannot recover.");
		}
		const OWTransform* parT2 = a->transform()->hostTransform();
		if (parT1 != parT2)
		{
			throw NMSLogicException(getActorDesc(a) + " transform has an invalid hostTransform. Cannot recover.");
		}
	}
}

static void validateSound(OWActor* a, OWSoundComponent*& sound)
{
	if (sound == nullptr)
		sound = new OWSoundComponent(); // Not all Actors have sound
}

static void validateRenderer(OWActor* a, const OWRenderer* rend)
{
	if (rend == nullptr)
	{
		throw NMSLogicException(getActorDesc(a) + " has no OWRenderer. Cannot recover.");
	}
}

size_t OWActorDiscrete::addComponents(const DiscreteEntity& newElement)
{
	mElements.push_back(newElement);
	DiscreteEntity& elm = mElements.back();
	validateCollider(this, elm.coll);
	validatePhysics(this, elm.phys);
	validateMeshComponent(this, elm.mesh);
	validateRenderer(this, elm.rend);
	validateTransform(this, elm.trans);
	validateSound(this, elm.sound);
	size_t retval = mElements.size() - 1;
	elm.coll->componentIndex(retval);
	return retval;
}

void OWActorDiscrete::doSetupActor()
{
	AABB b;
	for (int i = 0; i < mElements.size(); i++)
	{
		DiscreteEntity& elm = mElements[i];
		elm.mesh->setup();
		AABB b1;
		elm.coll->points(b1);
		if (elm.coll->collisionType() != OWCollider::CollisionType::Permeable)
			scene()->addCollider(elm.coll, this, i);
		OWRenderData rd = elm.mesh->renderData(b1);
		b = b | b1;

		if (!elm.rend->mSSBO.locked(GPUBufferObject::BillboardSize))
		{
			elm.rend->mSSBO.append(elm.trans->drawSize(elm.mesh->drawType()), GPUBufferObject::BillboardSize);
		}
		if (!elm.rend->mSSBO.locked(GPUBufferObject::Model))
		{
			elm.rend->mSSBO.append(elm.trans->modelMatrix(), GPUBufferObject::Model);
		}
		if (!elm.rend->mSSBO.locked(GPUBufferObject::Position))
		{
			const glm::vec4& p = glm::vec4(elm.trans->worldPosition(), 0);
			elm.rend->mSSBO.append(p, GPUBufferObject::Position);
		}
		if (!elm.rend->mSSBO.locked(GPUBufferObject::Colour))
		{
			elm.rend->mSSBO.append(elm.colour, GPUBufferObject::Colour);
		}
		elm.rend->setup(rd);
	}
	bounds(b);
}

void OWActorDiscrete::doRender(const glm::mat4& proj,
	const glm::mat4& view, const glm::vec3& cameraPos) 
{
	for (auto& elm: mElements)
	{
		elm.rend->render(proj, view, cameraPos);
	}
}

void OWActorDiscrete::doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required)
{
	if (ndx < mElements.size())
	{
		DiscreteEntity& elm = mElements[ndx];
		required.phys = elm.phys;
		required.mesh = elm.mesh;
		required.sound = elm.sound;
		required.trans = elm.trans;
	}
}

void OWActorDiscrete::doCollided(const OWCollider& component, const OWCollider& otherComponent)
{
	throw NMSNotYetImplementedException("OWActorDiscrete::doCollided() not yet implemented.");
}

size_t OWActorNCom1Ren::addComponents(const NCom1RenElement& newElement)
{
	mElements.push_back(newElement);
	NCom1RenElement& elm = mElements.back();
	validateCollider(this, elm.coll);
	validatePhysics(this, elm.phys);
	validateMeshComponent(this, elm.mesh);
	validateTransform(this, elm.trans);
	validateSound(this, elm.sound);
	size_t retval = mElements.size() - 1;
	elm.coll->componentIndex(retval);
	return retval;
}

void OWActorNCom1Ren::doSetupActor()
{
	validateRenderer(this, mRenderer);

	OWRenderData rd;
	AABB b;
	for (int i = 0; i < mElements.size(); i++)
	{
		const NCom1RenElement& elm = mElements[i];
		elm.mesh->setup();
		AABB b1;
		OWRenderData rd_elm = elm.mesh->renderData(b1);
		rd.add(rd_elm, true);
		elm.coll->points(b1);
		if (elm.coll->collisionType() != OWCollider::CollisionType::Permeable)
		{
			scene()->addCollider(elm.coll, this, i);
		}
		b = b | b1;
		glm::vec3 jfw3 = elm.trans->worldPosition();
		glm::mat4 jfw4 = elm.trans->modelMatrix();
		if (!mRenderer->mSSBO.locked(GPUBufferObject::BillboardSize))
		{
			mRenderer->mSSBO.append(elm.trans->drawSize(elm.mesh->drawType()), GPUBufferObject::BillboardSize);

		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Model))
		{
			mRenderer->mSSBO.append(elm.trans->modelMatrix(), GPUBufferObject::Model);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Position))
		{
			const glm::vec4& p = glm::vec4(elm.trans->worldPosition(), 0);
			mRenderer->mSSBO.append(p, GPUBufferObject::Position);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Colour))
		{
			mRenderer->mSSBO.append(elm.colour, GPUBufferObject::Colour);
		}
	}
	mRenderer->setup(rd);
	bounds(b);
}

void OWActorNCom1Ren::doRender(const glm::mat4& proj,
	const glm::mat4& view, const glm::vec3& cameraPos) 
{
	mRenderer->render(proj, view, cameraPos);
}

void OWActorNCom1Ren::doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required)
{
	if (ndx < mElements.size())
	{
		NCom1RenElement& elm = mElements[ndx];
		required.phys = elm.phys;
		required.mesh = elm.mesh;
		required.sound = elm.sound;
		required.trans = elm.trans;
	}
}

void OWActorNCom1Ren::doCollided(const OWCollider& component, const OWCollider& otherComponent)
{
	throw NMSNotYetImplementedException("OWActorDiscrete::doCollided() not yet implemented.");
}

size_t OWActorMutableParticle::addComponents(const MutableParticleElement& newElement)
{
	mElements.push_back(newElement);
	MutableParticleElement& elm = mElements.back();
	validateCollider(this, elm.coll);
	validatePhysics(this, elm.phys);
	validateTransform(this, elm.trans);
	size_t retval = mElements.size() - 1;
	elm.coll->componentIndex(retval);
	return retval;
}

void OWActorMutableParticle::doSetupActor()
{
	validateRenderer(this, mRenderer);
	if (mRenderer->mSSBO.bufferStyle() == GPUBufferObject::BufferStyle::Uniform)
		throw NMSLogicException("OWActorMutableParticle::doSetupActor(). Renderer BufferStyle must be SSBO.\n");
	validateMeshComponent(this, mMeshTemplate);
	validateSound(this, mSound);
	mMeshTemplate->setup();
	AABB b = AABB(0);
	OWRenderData rd = mMeshTemplate->renderData(b);
	for (int i = 0; i < mElements.size(); i++)
	{
		MutableParticleElement& elm = mElements[i];
		AABB b_moved = b;
		const glm::vec4& p = glm::vec4(elm.trans->worldPosition(), 0);
		b_moved.move(p);
		elm.coll->points(b_moved);
		if (elm.coll->collisionType() != OWCollider::CollisionType::Permeable)
		{
			scene()->addCollider(elm.coll, this, i);
		}
		b = b | b_moved;
		if (!mRenderer->mSSBO.locked(GPUBufferObject::BillboardSize))
		{
			mRenderer->mSSBO.append(elm.trans->drawSize(mMeshTemplate->drawType()), GPUBufferObject::BillboardSize);

		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Model))
		{
			mRenderer->mSSBO.append(elm.trans->modelMatrix(), GPUBufferObject::Model);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Position))
		{
			mRenderer->mSSBO.append(p, GPUBufferObject::Position);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Colour))
		{
			mRenderer->mSSBO.append(elm.colour, GPUBufferObject::Colour);
		}
	}
	mRenderer->setup(rd);
	bounds(b);
}

void OWActorMutableParticle::doRender(const glm::mat4& proj,
	const glm::mat4& view, const glm::vec3& cameraPos)
{
	mRenderer->render(proj, view, cameraPos);
}

void OWActorMutableParticle::doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required)
{
	if (ndx < mElements.size())
	{
		MutableParticleElement& elm = mElements[ndx];
		required.phys = elm.phys;
		required.mesh = mMeshTemplate;
		required.sound = mSound;
		required.trans = elm.trans;
	}
}

void OWActorMutableParticle::renderer(OWRenderer* newValue)
{
	mRenderer = newValue;
}

void OWActorMutableParticle::sound(OWSoundComponent* newValue)
{
	mSound = newValue;
}

void OWActorMutableParticle::doCollided(const OWCollider& component, const OWCollider& otherComponent)
{
	throw NMSNotYetImplementedException("OWActorDiscrete::doCollided() not yet implemented.");
}

OWActorImmutableParticle::OWActorImmutableParticle(Scene* _scene,
	const std::string& _name, OWActor* _hostActor)
: OWActor(_scene, _name, _hostActor) 
{

}

void OWActorImmutableParticle::doSetupActor()
{
	validateRenderer(this, mRenderer);
	validateMeshComponent(this, mMeshTemplate);
	validatePhysics(this, mPhysics);
	validateSound(this, mSound);
	mMeshTemplate->setup();
	AABB b;
	OWRenderData rd = mMeshTemplate->renderData(b);
	mRenderer->setup(rd);
	bounds(b);
}

void OWActorImmutableParticle::doRender(const glm::mat4& proj,
	const glm::mat4& view, const glm::vec3& cameraPos)
{
	mRenderer->render(proj, view, cameraPos);
}

void OWActorImmutableParticle::renderer(OWRenderer* newValue)
{
	mRenderer = newValue;
}

void OWActorImmutableParticle::sound(OWSoundComponent* newValue)
{
	mSound = newValue;
}

void OWActorImmutableParticle::doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required)
{
	if (ndx == 0)
	{
		required.mesh = mMeshTemplate;
		required.phys = mPhysics;
		required.sound = mSound;
	}
}

void OWActorImmutableParticle::doCollided(const OWCollider& component, const OWCollider& otherComponent)
{
	throw NMSNotYetImplementedException("OWActorDiscrete::doCollided() not yet implemented.");
}

