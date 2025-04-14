#include "OWActor.h"
#include "../Core/Scene.h"
#include "../Renderers/MeshRenderer.h"

static std::string getActorDesc(OWActor* a)
{
	const OWActor& aref = *a;
	return std::string(typeid(aref).name()) + "[" + a->name() + "]";
}

OWActor::OWActor(Scene* _scene, const std::string& _name, OWActor* _hostActor)
	: mScene(_scene), mName(_name), mHostActor(_hostActor)
{
	_scene->addActor(this);
	if (_hostActor == nullptr)
	{
		if (this->transform() != nullptr)
		{
			// jfw can this be ever true?
			throw NMSLogicException(getActorDesc(this) + " already has a transform.");
		}
	}
	else
	{
		if (_hostActor->transform() == nullptr)
		{
			throw NMSLogicException("OWActor [" + _hostActor->name()
				+ "] must have a transform before creating sub Actors. Cannot recover.");
		}
		// jfw Does this overwrite an existing transform?
		//OWTransform* trans = new OWTransform();
		//trans->parentTransform(_hostActor->transform());
	}
}

#ifdef _DEBUG
bool OWActor::debugInclude() const
{
	//return name() == "All Dice";
	//return name() == "Fullscreen";
	return true;
}
#endif

void OWActor::transform(OWTransform* newValue) 
{
	if (mActorTransform != nullptr)
	{
		throw NMSLogicException(getActorDesc(this) + " already has a transform.");
	}
	mActorTransform = newValue;
	if (mHostActor != nullptr)
	{
		mActorTransform->parentTransform(mHostActor->transform());
	}
}

void OWActor::setup()
{
	if (!mSetup)
	{
		if (mScriptor.actor() == nullptr)
			mScriptor.actor(this);
		if (this->transform() == nullptr)
		{
			throw NMSLogicException(getActorDesc(this) + " must have a transform before setup is called.");
		}
#ifdef _DEBUG
		if (debugInclude())
		{
#endif
			doSetupActor();
#ifdef _DEBUG
		}
#endif
			mSetup = true;
	}
}

void OWActor::preTick()
{
	copyCurrentToPrevious();
	doPreTick();
	// Placeholder called on the main thread. OWActor should quickly 
	// create a background thread to do stuff while render is happenening.
}

void OWActor::tick(float dt) 
{
	mScriptor.tick(dt);
	doTick(dt);
}

void OWActor::postTick()
{
	doPostTick();
}

void OWActor::preRender()
{
	copyCurrentToPrevious();
	doPreRender();
	// Placeholder called on the main thread. OWActor should quickly 
	// create a background thread to do stuff while render is happenening.
}

void OWActor::interpolatePhysics(float totalTime, float alpha, float fixedTimeStep)
{
#ifdef _DEBUG
	if (!debugInclude())
		return;
#endif
	doInterpolatePhysics(totalTime, alpha, fixedTimeStep);
}

void OWActor::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::vec3& cameraPos)
{
#ifdef _DEBUG
	if (!debugInclude())
		return;
#endif
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

static void validateTransform(OWActor* a, OWTransform* trans)
{
	if ((trans->parentTransform() != nullptr) && (trans->parentTransform() != a->transform()))
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
		const OWTransform* parT2 = a->transform()->parentTransform();
		if (parT1 != parT2)
		{
			throw NMSLogicException(getActorDesc(a) + " transform has an invalid hostTransform. Cannot recover.");
		}
	}
}

static void validatePhysics(OWActor* a, OWPhysics*& phys)
{
	if (phys == nullptr)
	{
		phys = new OWPhysics(a->transform()); // Physics is often the default.
	}
	validateTransform(a, phys->transform());
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
	validatePhysics(this, elm.physics);
	validateMeshComponent(this, elm.mesh);
	validateRenderer(this, elm.rend);
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
			elm.rend->mSSBO.append(elm.physics->transform()->drawSize(elm.mesh->drawType()), GPUBufferObject::BillboardSize);
		}
		if (!elm.rend->mSSBO.locked(GPUBufferObject::Model))
		{
			elm.rend->mSSBO.append(elm.physics->transform()->modelMatrix(), GPUBufferObject::Model);
		}
		if (!elm.rend->mSSBO.locked(GPUBufferObject::Position))
		{
			const glm::vec4& p = glm::vec4(elm.physics->transform()->worldPosition(), 0);
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

void OWActorDiscrete::doPreTick()
{
}

void OWActorDiscrete::doTick(float dt)
{
	for (int i = 0; i < mElements.size(); i++)
	{
		DiscreteEntity& elm = mElements[i];
		elm.physics->tick(dt);
	}
}

void OWActorDiscrete::doPostTick()
{
}

void OWActorDiscrete::doPreRender()
{
}

void OWActorDiscrete::doPostRender()
{
}

void OWActorDiscrete::doRender(const glm::mat4& proj,
	const glm::mat4& view, const glm::vec3& cameraPos) 
{
	for (auto& elm: mElements)
	{
		if (elm.mesh->active())
			elm.rend->render(proj, view, cameraPos);
	}
}

void OWActorDiscrete::doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required)
{
	if (ndx < mElements.size())
	{
		DiscreteEntity& elm = mElements[ndx];
		required.phys = elm.physics;
		required.mesh = elm.mesh;
		required.sound = elm.sound;
	}
}

void OWActorDiscrete::doInterpolatePhysics(float totalTime, float alpha, float fixedTimeStep)
{
	for (int i = 0; i < mElements.size(); i++)
	{
		DiscreteEntity& elm = mElements[i];
		elm.physics->interpolate(totalTime, alpha, fixedTimeStep);
		if (!elm.rend->mSSBO.locked(GPUBufferObject::Model))
		{
			glm::mat4 m = elm.physics->renderTransform()->modelMatrix();
			float* f = glm::value_ptr(m);
			elm.rend->mSSBO.updateData(f, GPUBufferObject::Model, 0);
		}
		if (!elm.rend->mSSBO.locked(GPUBufferObject::Position))
		{
			glm::vec4 m = glm::vec4(elm.physics->renderTransform()->localPosition(), 0);
			float* f = glm::value_ptr(m);
			elm.rend->mSSBO.updateData(f, GPUBufferObject::Position, 0);
		}
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
	validatePhysics(this, elm.physics);
	validateMeshComponent(this, elm.mesh);
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
		glm::vec3 jfw3 = elm.physics->transform()->worldPosition();
		glm::mat4 jfw4 = elm.physics->transform()->modelMatrix();
		if (!mRenderer->mSSBO.locked(GPUBufferObject::BillboardSize))
		{
			mRenderer->mSSBO.append(elm.physics->transform()->drawSize(elm.mesh->drawType()), GPUBufferObject::BillboardSize);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Model))
		{
			mRenderer->mSSBO.append(elm.physics->transform()->modelMatrix(), GPUBufferObject::Model);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Position))
		{
			const glm::vec4& p = glm::vec4(elm.physics->transform()->worldPosition(), 0);
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

void OWActorNCom1Ren::doPreTick()
{
}

void OWActorNCom1Ren::doTick(float dt)
{
	for (int i = 0; i < mElements.size(); i++)
	{
		NCom1RenElement& elm = mElements[i];
		elm.physics->tick(dt);
	}
}

void OWActorNCom1Ren::doPostTick()
{
}

void OWActorNCom1Ren::doPreRender()
{
}

void OWActorNCom1Ren::doPostRender()
{
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
		required.phys = elm.physics;
		required.mesh = elm.mesh;
		required.sound = elm.sound;
	}
}

void OWActorNCom1Ren::doInterpolatePhysics(float totalTime, float alpha, float fixedTimeStep)
{
	for (int i = 0; i < mElements.size(); i++)
	{
		NCom1RenElement& elm = mElements[i];
		elm.physics->interpolate(totalTime, alpha, fixedTimeStep);
		if (!mRenderer->mSSBO.locked(GPUBufferObject::BillboardSize))
		{
			glm::vec4 m = glm::vec4(elm.physics->transform()->drawSize(elm.mesh->drawType()), 0, 0);
			float* f = glm::value_ptr(m);
			mRenderer->mSSBO.updateData(f, GPUBufferObject::BillboardSize, i);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Model))
		{
			glm::mat4 m = elm.physics->renderTransform()->modelMatrix();
			float* f = glm::value_ptr(m);
			mRenderer->mSSBO.updateData(f, GPUBufferObject::Model, i);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Position))
		{
			glm::vec4 m = glm::vec4(elm.physics->renderTransform()->localPosition(), 0);
			float* f = glm::value_ptr(m);
			mRenderer->mSSBO.updateData(f, GPUBufferObject::Position, i);
		}
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
	validatePhysics(this, elm.physics);
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
		const glm::vec4& p = glm::vec4(elm.physics->transform()->worldPosition(), 0);
		b_moved.move(p);
		elm.coll->points(b_moved);
		if (elm.coll->collisionType() != OWCollider::CollisionType::Permeable)
		{
			scene()->addCollider(elm.coll, this, i);
		}
		b = b | b_moved;
		if (!mRenderer->mSSBO.locked(GPUBufferObject::BillboardSize))
		{
			mRenderer->mSSBO.append(elm.physics->transform()->drawSize(mMeshTemplate->drawType()), GPUBufferObject::BillboardSize);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Model))
		{
			mRenderer->mSSBO.append(elm.physics->transform()->modelMatrix(), GPUBufferObject::Model);
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

void OWActorMutableParticle::doPreTick()
{
}

void OWActorMutableParticle::doTick(float dt)
{
	for (int i = 0; i < mElements.size(); i++)
	{
		MutableParticleElement& elm = mElements[i];
		elm.physics->tick(dt);
	}
}

void OWActorMutableParticle::doPostTick()
{
}

void OWActorMutableParticle::doPreRender()
{
}

void OWActorMutableParticle::doPostRender()
{
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
		required.phys = elm.physics;
		required.mesh = mMeshTemplate;
		required.sound = mSound;
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

void OWActorMutableParticle::doInterpolatePhysics(float totalTime, float alpha, float fixedTimeStep) 
{
	for (int i = 0; i < mElements.size(); i++)
	{
		MutableParticleElement& elm = mElements[i];
		elm.physics->interpolate(totalTime, alpha, fixedTimeStep);
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Model))
		{
			glm::mat4 m = elm.physics->renderTransform()->modelMatrix();
			float* f = glm::value_ptr(m);
			mRenderer->mSSBO.updateData(f, GPUBufferObject::Model, i);
		}
		if (!mRenderer->mSSBO.locked(GPUBufferObject::Position))
		{
			glm::vec4 m = glm::vec4(elm.physics->renderTransform()->localPosition(), 0);
			float* f = glm::value_ptr(m);
			mRenderer->mSSBO.updateData(f, GPUBufferObject::Position, i);
		}
	}
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

void OWActorImmutableParticle::doPreTick()
{
}

void OWActorImmutableParticle::doTick(float dt)
{
	// Nothing changes, do nothing
}

void OWActorImmutableParticle::doPostTick()
{
}

void OWActorImmutableParticle::doPreRender()
{
}

void OWActorImmutableParticle::doPostRender()
{
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
	throw NMSLogicException("OWActorImmutableParticle::doCollided() should not collide with anything.");
}

