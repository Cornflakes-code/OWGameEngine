#include "OWActor.h"
#include "../Core/Scene.h"
#include "../Core/ErrorHandling.h"
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

void OWActor::getScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required)
{
	return;
	required.phys = mPhysics[ndx];
	required.mesh = mMeshes[ndx > mMeshes.size() ? 0: ndx];
	required.sound = mSounds[ndx > mSounds.size() ? 0 : ndx];
}

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

void OWActor::preModifyMesh(OWMeshComponent* existingMesh)
{
	// reduce bounds by the size of existingMesh
}

void OWActor::postModifyMesh(OWMeshComponent* modifiedMesh)
{
	doSetupActor(modifiedMesh);
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
			doSetupActor(nullptr);
#ifdef _DEBUG
		}
#endif
		mSetup = true;
	}
}


void OWActor::preTick()
{
	if (active())
	{
		copyCurrentToPrevious();
		doPreTick();
		// Placeholder called on the main thread. OWActor should quickly 
		// create a background thread to do stuff while render is happenening.
	}
}

void OWActor::tick(float dt)
{
	if (active())
	{
		mScriptor.tick(dt);
		OWSize sz = physicsSize();
		for (OWSize i = 0; i < sz; i++)
		{
			OWPhysics* physics = getPhysics(i);
			physics->tick(dt);
			getCollider(i)->position(physics->transform()->worldPosition());
		}
	}
}

void OWActor::postTick()
{
	if (active())
	{
		doPostTick();
	}
}

void OWActor::preRender()
{
	if (active())
	{
		copyCurrentToPrevious();
		doPreRender();
		// Placeholder called on the main thread. OWActor should quickly 
		// create a background thread to do stuff while render is happenening.
	}
}

void OWActor::interpolatePhysics(float totalTime, float alpha, float fixedTimeStep)
{
	if (active())
	{
#ifdef _DEBUG
		if (!debugInclude())
			return;
#endif
		{
			volatile OWSize sz = physicsSize();
			for (OWSize i = 0; i < sz; i++)
			{
				OWPhysics* physics = getPhysics(i);
				OWRenderer* rend = getRenderer(i);
				physics->interpolate(totalTime, alpha, fixedTimeStep);

				// If each iteration of the loop uses a different renderer then we want to reset the index into the SSBO
				OWSize ssboIndex = renderersSize() == 1 ? i : 0;

				if (!rend->mSSBO.locked(GPUBufferObject::BillboardSize))
				{
					glm::vec4 m = glm::vec4(physics->transform()->drawSize(getMeshComponent(i)->drawType()), 0, 0);
					float* f = glm::value_ptr(m);
					rend->mSSBO.updateData(f, GPUBufferObject::BillboardSize, ssboIndex);
				}
				if (!rend->mSSBO.locked(GPUBufferObject::Model))
				{
					glm::mat4 m = physics->renderTransform()->modelMatrix();
					glm::mat4 jfw;
					jfw[0] = { 10, 0, 0, 0 };
					jfw[1] = { 0, 7.07107, 7.07107, 0 };
					jfw[2] = { 0, -7.07107, 7.07107, 0 };
					jfw[3] = { 0.001312, 69.0021, 24.0011, 1 };
//					m = jfw;
					float* f = glm::value_ptr(m);
					rend->mSSBO.updateData(f, GPUBufferObject::Model, ssboIndex);
				}
				if (!rend->mSSBO.locked(GPUBufferObject::Position))
				{
					glm::vec4 m = glm::vec4(physics->renderTransform()->worldPosition(), 0);
					float* f = glm::value_ptr(m);
					rend->mSSBO.updateData(f, GPUBufferObject::Position, ssboIndex);
				}
				if (!rend->mSSBO.locked(GPUBufferObject::Colour))
				{
					glm::vec4 m = getColour(i);
					float* f = glm::value_ptr(m);
					rend->mSSBO.updateData(f, GPUBufferObject::Colour, ssboIndex);
				}
				if (!rend->mSSBO.locked(GPUBufferObject::Anything))
				{
					glm::vec4 m = glm::vec4(physics->renderTransform()->worldPosition(), 0);
					float* f = glm::value_ptr(m);
					rend->mSSBO.updateData(f, GPUBufferObject::Anything, ssboIndex);
				}
			}
		}
	}
}

void OWActor::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::vec3& cameraPos)
{
	if (active())
	{
#ifdef _DEBUG
		if (!debugInclude())
			return;
#endif
		if (mSetup && active())
		{
			OWSize sz = renderersSize();
			for (OWSize i = 0; i < sz; i++)
			{
				if (getMeshComponent(i)->active())
					getRenderer(i)->render(proj, view, cameraPos);
			}
		}
	}
}

void OWActor::postRender()
{
	doPostRender();
	// Placeholder called on the main thread. OWActor should quickly tidy
	// up whatever prePender() did.
}

void OWActor::collided(const OWCollider& component, const OWCollider& otherComponent)
{
	doCollided(component, otherComponent);
}

const glm::vec4& OWActor::getColour(OWSize ndx)
{
	return (ndx < mColours.size()) ? mColours[ndx] : mColours[0];
}

void OWActor::setColour(const glm::vec4& colour, OWSize ndx)
{
	mColours[ndx] = colour;
}

OWCollider* OWActor::getCollider(OWSize ndx)
{
	return (ndx < mColliders.size()) ? mColliders[ndx] : mColliders[0];
}

void OWActor::setCollider(OWCollider* coll, OWSize ndx)
{
	mColliders[ndx] = coll;
}

OWPhysics* OWActor::getPhysics(OWSize ndx)
{
	return (ndx < mPhysics.size()) ? mPhysics[ndx] : mPhysics[0];
}

void OWActor::setPhysics(OWPhysics* phys, OWSize ndx)
{
	mPhysics[ndx] = phys;
}

OWMeshComponentBase* OWActor::getMeshComponent(OWSize ndx)
{
	return (ndx < mMeshes.size()) ? mMeshes[ndx] : mMeshes[0];
}

void OWActor::setMeshComponent(OWMeshComponentBase* mesh, OWSize ndx)
{
	mMeshes[ndx] = mesh;
}

OWRenderer* OWActor::getRenderer(OWSize ndx)
{
	return (ndx < mRenderers.size()) ? mRenderers[ndx] : mRenderers[0];
}

void OWActor::setRenderer(OWRenderer* rend, OWSize ndx)
{
	mRenderers[ndx] = rend;
}

OWSoundComponent* OWActor::getSound(OWSize ndx)
{
	return (ndx < mSounds.size()) ? mSounds[ndx] : mSounds[0];
}

void OWActor::setSound(OWSoundComponent* sound, OWSize ndx)
{
	mSounds[ndx] = sound;
}

void OWActor::addColour(const glm::vec4& colour)
{
	mColours.push_back(colour);
}

void OWActor::addCollider(OWCollider* coll)
{
	mColliders.push_back(coll);
}

void OWActor::addPhysics(OWPhysics* phys)
{
	if (phys == nullptr)
		mPhysics.push_back(new OWPhysics(this->transform()));
	else
		mPhysics.push_back(phys);
}

void OWActor::addMeshComponent(OWMeshComponentBase* mesh)
{
	mMeshes.push_back(mesh);
}

void OWActor::addRenderer(OWRenderer* rend)
{
	mRenderers.push_back(rend);
}

void OWActor::addSound(OWSoundComponent* sound)
{
	if (sound == nullptr)
		mSounds.push_back(new OWSoundComponent());
	else
		mSounds.push_back(sound);
}

void OWActor::copyCurrentToPrevious()
{
	for (auto& elm : mPhysics)
	{
		elm->copyCurrentToPrevious();
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
	if (trans->parentTransform() == nullptr)
	{
		trans->parentTransform(a->transform());
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

static void validatePhysics(OWActor* a, OWPhysics* phys)
{
	if (phys == nullptr)
	{
		phys = new OWPhysics(a->transform()); // Physics is often the default.
	}
	validateTransform(a, phys->transform());
}

static void validateSound(OWActor* a, OWSoundComponent* sound)
{
}

static void validateRenderer(OWActor* a, const OWRenderer* rend)
{
	if (rend == nullptr)
	{
		throw NMSLogicException(getActorDesc(a) + " has no OWRenderer. Cannot recover.");
	}
}

OWSize OWActorDiscrete::addComponents(const DiscreteEntity& newElement)
{
	addColour(newElement.colour);
	addCollider(newElement.coll);
	addPhysics(newElement.physics);
	addMeshComponent(newElement.mesh);
	addRenderer(newElement.rend);
	addSound(newElement.sound);
	
	OWSize back = static_cast<OWSize>(meshesSize()) - 1;
	OWCollider* coll = getCollider(back);
	validateCollider(this, coll);
	validatePhysics(this, getPhysics(back));
	validateMeshComponent(this, getMeshComponent(back));
	validateRenderer(this, getRenderer(back));
	validateSound(this, getSound(back));
	coll->componentIndex(back);
	return back;
}

void OWActorDiscrete::doCollided(const OWCollider& component, const OWCollider& otherComponent)
{
	throw NMSNotYetImplementedException("OWActorDiscrete::doCollided() not yet implemented.");
}

void OWActorDiscrete::doSetupActor(OWMeshComponentBase* target)
{
	OWSize sz = static_cast<OWSize>(meshesSize());
	AABB cumulativeSize = bounds();
	for (OWSize i = 0; i < sz; i++)
	{
		OWMeshComponentBase* mesh = getMeshComponent(i);
		OWRenderer* rend = getRenderer(i);
		OWCollider* coll = getCollider(i);
		if (target != nullptr)
		{
			if (target == mesh)
			{
				mesh->setup();
				AABB elm_bounds;
				OWRenderData rd = getMeshComponent(i)->renderData(elm_bounds);
				cumulativeSize |= elm_bounds;
				coll->points(elm_bounds);
				rend->setupCompleted(false);
				rend->setup(rd);
				bounds(cumulativeSize);
				return;
			}
			else
			{
				continue;
			}
		}
		mesh->setup();
		AABB elm_bounds;
		OWRenderData rd = getMeshComponent(i)->renderData(elm_bounds);
		OWPhysics* physics = getPhysics(i);
		elm_bounds.scale(physics->transform()->scale());
		if (coll->collisionType() != OWCollider::CollisionType::Permeable)
			scene()->addCollider(coll, this, i);
		coll->points(elm_bounds);
		cumulativeSize = cumulativeSize | elm_bounds;
		if (!rend->mSSBO.locked(GPUBufferObject::BillboardSize))
		{
			rend->mSSBO.append(physics->transform()->drawSize(mesh->drawType()), GPUBufferObject::BillboardSize);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Model))
		{
			rend->mSSBO.append(physics->transform()->modelMatrix(), GPUBufferObject::Model);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Position))
		{
			const glm::vec4& p = glm::vec4(physics->transform()->worldPosition(), 0);
			rend->mSSBO.append(p, GPUBufferObject::Position);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Colour))
		{
			rend->mSSBO.append(getColour(i), GPUBufferObject::Colour);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Anything))
		{
			rend->mSSBO.append(glm::vec4(0), GPUBufferObject::Anything);
		}
		rend->setup(rd);
	}
	bounds(cumulativeSize);
}

OWSize OWActorNCom1Ren::addComponents(const NCom1RenElement& newElement)
{
	addColour(newElement.colour);
	addCollider(newElement.coll);
	addMeshComponent(newElement.mesh);
	addPhysics(newElement.physics);
	addSound(newElement.sound);

	OWSize back = static_cast<OWSize>(meshesSize()) - 1;
	OWCollider* coll = getCollider(back);
	validateCollider(this, coll);
	validatePhysics(this, getPhysics(back));
	validateMeshComponent(this, getMeshComponent(back));
	validateSound(this, getSound(back));

	coll->componentIndex(back);
	return back;
}

void OWActorNCom1Ren::doSetupActor(OWMeshComponentBase* target)
{
	if (renderersSize() != 1)
		throw NMSLogicException("OWActorNCom1Ren::doSetupActor(). Must have exactly one renderer.\n");
	OWRenderer* rend = getRenderer(0);
	if (target != nullptr)
	{
		// The mesh has changed and probably too hard to splice into existing meshes. Basically it is another doSetupActor()
		bounds(AABB());// clear the bounds;
		rend->mSSBO = GPUBufferObject();
	}
	validateRenderer(this, rend);
	OWSize sz = static_cast<OWSize>(meshesSize());
	OWRenderData rd;
	AABB b = bounds();
	for (OWSize i = 0; i < sz; i++)
	{
		OWMeshComponentBase* mesh = getMeshComponent(i);
		OWCollider* coll = getCollider(i);
		OWPhysics* physics = getPhysics(i);
		mesh->setup();
		AABB b1;
		OWRenderData rd_elm = mesh->renderData(b1);
		rd.add(rd_elm, true);
		coll->points(b1);
		if (coll->collisionType() != OWCollider::CollisionType::Permeable)
		{
			scene()->addCollider(coll, this, i);
		}
		b = b | b1;
		glm::vec3 jfw3 = physics->transform()->worldPosition();
		glm::mat4 jfw4 = physics->transform()->modelMatrix();
		if (!rend->mSSBO.locked(GPUBufferObject::BillboardSize))
		{
			rend->mSSBO.append(physics->transform()->drawSize(mesh->drawType()), GPUBufferObject::BillboardSize);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Model))
		{
			rend->mSSBO.append(physics->transform()->modelMatrix(), GPUBufferObject::Model);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Position))
		{
			const glm::vec4& p = glm::vec4(physics->transform()->worldPosition(), 0);
			rend->mSSBO.append(p, GPUBufferObject::Position);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Colour))
		{
			rend->mSSBO.append(getColour(i), GPUBufferObject::Colour);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Anything))
		{
			rend->mSSBO.append(glm::vec4(0), GPUBufferObject::Anything);
		}
	}
	rend->setup(rd);
	bounds(b);
}

void OWActorNCom1Ren::doCollided(const OWCollider& component, const OWCollider& otherComponent)
{
	throw NMSNotYetImplementedException("OWActorNCom1Ren::doCollided() not yet implemented.");
}

OWSize OWActorMutableParticle::addComponents(const MutableParticleElement& newElement)
{
	addColour(newElement.colour);
	addCollider(newElement.coll);
	addPhysics(newElement.physics);

	OWSize back = physicsSize() - 1;
	OWCollider* coll = getCollider(back);
	validateCollider(this, coll);
	validatePhysics(this, getPhysics(back));
	coll->componentIndex(back);
	return back;
}

void OWActorMutableParticle::doSetupActor(OWMeshComponentBase* target)
{
	if (soundsSize() == 0)
		addSound(new OWSoundComponent());
	if (soundsSize() != 1)
		throw NMSLogicException("OWActorMutableParticle::doSetupActor(). Must have exactly one sound.\n");
	validateSound(this, getSound(0));

	if (renderersSize() != 1)
		throw NMSLogicException("OWActorMutableParticle::doSetupActor(). Must have exactly one renderer.\n");
	OWRenderer* rend = getRenderer(0);
	if (rend->mSSBO.bufferStyle() == GPUBufferObject::BufferStyle::Uniform)
		throw NMSLogicException("OWActorMutableParticle::doSetupActor(). Renderer BufferStyle must be SSBO.\n");
	validateRenderer(this, rend);

	if (meshesSize() != 1)
		throw NMSLogicException("OWActorMutableParticle::doSetupActor(). Must have exactly one mesh.\n");

	if (target != nullptr)
	{
		// The mesh has changed. Basically it is another doSetupActor()
		bounds(AABB());// clear the bounds;
		rend->mSSBO = GPUBufferObject();
	}
	OWMeshComponentBase* mesh = getMeshComponent(0);
	validateMeshComponent(this, mesh);
	validateSound(this, getSound(0));
	mesh->setup();
	AABB b = AABB(0);
	OWRenderData rd = mesh->renderData(b);
	AABB cumulativeSize = b;
	OWSize num = static_cast<OWSize>(physicsSize());
	for (OWSize i = 0; i < num; i++)
	{
		OWCollider* coll = getCollider(i);
		OWPhysics* physics = getPhysics(i);

		AABB elm_bounds = b;
		elm_bounds.scale(physics->transform()->scale());
		const glm::vec3& p = physics->transform()->worldPosition();
		coll->points(p, elm_bounds.size(), glm::vec3(0));
		if (coll->collisionType() != OWCollider::CollisionType::Permeable)
		{
			scene()->addCollider(coll, this, i);
		}
		cumulativeSize = cumulativeSize | elm_bounds;
		if (!rend->mSSBO.locked(GPUBufferObject::BillboardSize))
		{
			rend->mSSBO.append(physics->transform()->drawSize(mesh->drawType()), GPUBufferObject::BillboardSize);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Model))
		{
			rend->mSSBO.append(physics->transform()->modelMatrix(), GPUBufferObject::Model);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Position))
		{
			rend->mSSBO.append(p, GPUBufferObject::Position);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Colour))
		{
			rend->mSSBO.append(getColour(i), GPUBufferObject::Colour);
		}
		if (!rend->mSSBO.locked(GPUBufferObject::Anything))
		{
			rend->mSSBO.append(glm::vec4(0), GPUBufferObject::Anything);
		}
	}
	rend->setup(rd);
	bounds(cumulativeSize);
}

void OWActorMutableParticle::doCollided(const OWCollider& component, const OWCollider& otherComponent)
{
	throw NMSNotYetImplementedException("OWActorDiscrete::doCollided() not yet implemented.");
}

OWActorImmutableParticle::OWActorImmutableParticle(Scene* _scene,
	const std::string& _name, OWActor* _hostActor)
: OWActor(_scene, _name, _hostActor) 
{
	// The concept of OWActorImmutableParticle needs looking at
}

OWSize OWActorImmutableParticle::addComponents(const ImmutableParticleElement& newElement)
{
	addColour(newElement.colour);
	addMeshComponent(newElement.mesh);
	addRenderer(newElement.rend);
	addSound(newElement.sound);
	OWSize back = static_cast<OWSize>(meshesSize()) - 1;
	return back;
}

void OWActorImmutableParticle::doCollided(const OWCollider& component, const OWCollider& otherComponent)
{
}

void OWActorImmutableParticle::doSetupActor(OWMeshComponentBase* target)
{
	if (collidersSize() != 0)
		throw NMSLogicException("OWActorImmutableParticle::doSetupActor(). Cannot have colliders.\n");
	if (physicsSize() != 0)
		throw NMSLogicException("OWActorImmutableParticle::doSetupActor(). Cannot have physics.\n");

	if (coloursSize() != 1)
		throw NMSLogicException("OWActorImmutableParticle::doSetupActor(). Must have exactly one colour.\n");
	if (meshesSize() != 1)
		throw NMSLogicException("OWActorImmutableParticle::doSetupActor(). Must have exactly one mesh.\n");
	if (renderersSize() != 1)
		throw NMSLogicException("OWActorImmutableParticle::doSetupActor(). Must have exactly one renderer.\n");
	if (soundsSize() != 1)
		throw NMSLogicException("OWActorImmutableParticle::doSetupActor(). Must have exactly one sound.\n");
	OWRenderer* rend = getRenderer(0);
	if (target != nullptr)
	{
		// The mesh has changed and probably too hard to splice into existing meshes. Basically it is another doSetupActor()
		bounds(AABB());// clear the bounds;
		rend->mSSBO = GPUBufferObject();
	}
	validateRenderer(this, rend);
	OWMeshComponentBase* mesh = getMeshComponent(0);
	validateMeshComponent(this, mesh);
	validateSound(this, getSound(0));
	mesh->setup();
	AABB b;
	OWRenderData rd = mesh->renderData(b);
	rend->setup(rd);
	bounds(b);
}
