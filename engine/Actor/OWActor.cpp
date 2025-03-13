#include "OWActor.h"
#include "../Core/Scene.h"
#include "../Core/CollisionSystem.h"
#include "../Renderers/InstanceRenderer.h"
#include "../Renderers/MeshRenderer.h"

#ifdef _DEBUG
static std::string getActorDesc(OWActor* a)
{
	const OWActor& aref = *a;
	return std::string(typeid(aref).name()) + "[" + a->name() + "]";
}
#endif

OWActor::OWActor(Scene* _scene, const std::string& _name, OWActor* _hostActor)
	: mScene(_scene), mName(_name), mHostActor(_hostActor)
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
		doSetup();
		mSetup = true;
	}
}

void OWActor::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4 model,
	const glm::vec3& cameraPos)
{
	if (mSetup && active())
	{
		doRender(proj, view, model, cameraPos);
	}
}

static void validateCollider(OWActor* a, const OWCollider* coll)
{
#ifdef _DEBUG
	if (coll == nullptr)
	{
		throw NMSLogicException(getActorDesc(a) + " has no OWCollider.Cannot recover.");
	}
	if (coll->actor() != a)
	{
		throw NMSLogicException(getActorDesc(a) + " OWCollider has no owner. Cannot recover.");
	}
#endif
}

static void validatePhysics(OWActor* a, OWPhysics*& phys)
{
#ifdef _DEBUG
	if (phys == nullptr)
		phys = new OWPhysics(); // Physics is often the default.
#endif
}

static void validateMeshComponent(OWActor* a, const OWMeshComponentBase* mesh)
{
#ifdef _DEBUG
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
#endif
}

static void validateTransform(OWActor* a, OWTransform*& trans)
{
#ifdef _DEBUG
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
#endif
}

static void validateSound(OWActor* a, OWSoundComponent*& sound)
{
#ifdef _DEBUG
	if (sound == nullptr)
		sound = new OWSoundComponent(); // Not all Actors have sound
#endif
}

static void validateRenderer(OWActor* a, const OWRenderer* rend)
{
#ifdef _DEBUG
	if (rend == nullptr)
	{
		throw NMSLogicException(getActorDesc(a) + " has no OWRenderer. Cannot recover.");
	}
#endif
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

void OWActorDiscrete::doSetup()
{
	OWRenderData rd;
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

void OWActorNCom1Ren::doSetup()
{
	validateRenderer(this, mRenderer);
	// Each element of the SSO is position (glm::vec4) + colour (glm::vec4).
	//unsigned int ssoSize = 2 * sizeof(glm::vec4);
	unsigned int ssoSize = 1 * sizeof(glm::vec4) / sizeof(float);

	// mRenderer relies on the size not being changed
	mSSBO.resize(ssoSize * mElements.size());

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
			CollisionSystem::addCollider(elm.coll, this, i);
		}
		b = b | b1;
		const glm::vec3& p = elm.trans->worldPosition();
		size_t offset = i * ssoSize;
		mSSBO[offset + 0] = p.x;
		mSSBO[offset + 1] = p.y;
		mSSBO[offset + 2] = p.z;// 0.49308f;// p.z;
		mSSBO[offset + 3] = 0;
		/*
		mSSBO[offset + 4] = elm.colour.x;
		mSSBO[offset + 5] = elm.colour.y;
		mSSBO[offset + 6] = elm.colour.z;
		mSSBO[offset + 7] = elm.colour.w;
		*/
	}
	OWRenderer::SSBO ssbo(mSSBO);
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
	validateCollider(this, elm.coll);
	validatePhysics(this, elm.phys);
	validateTransform(this, elm.trans);
	size_t retval = mElements.size() - 1;
	elm.coll->componentIndex(retval);
	return retval;
}

void OWActorMutableParticle::doSetup()
{
	validateRenderer(this, mRenderer);
	validateMeshComponent(this, mMeshTemplate);
	validateSound(this, mSound);
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

void OWActorMutableParticle::renderer(OWRenderer* newValue)
{
	mRenderer = newValue;
}

void OWActorMutableParticle::sound(OWSoundComponent* newValue)
{
	mSound = newValue;
}


OWActorImmutableParticle::OWActorImmutableParticle(Scene* _scene, const std::string& _name, OWActor* _hostActor)
: OWActor(_scene, _name, _hostActor) 
{

}

void OWActorImmutableParticle::doSetup()
{
	validateRenderer(this, mRenderer);
	validateMeshComponent(this, mMeshTemplate);
	validatePhysics(this, mPhysics);
	validateSound(this, mSound);
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

