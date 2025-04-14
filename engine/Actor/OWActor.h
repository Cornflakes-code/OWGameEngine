#pragma once

#include "../Core/OWObject.h"
#include <vector>

#include "../OWEngine/OWEngine.h"
#include "../Scripting/OWActorScript.h"
#include "../Component/PhysicalComponent.h"
#include "../Component/MeshComponent.h"
#include "../Renderers/RendererBase.h"
#include "../Helpers/Transform.h"
#include "../Core/SoundManager.h"
#include "../Helpers/Collider.h"

class Scene;

class OWENGINE_API OWActor
{
public:
	OWActor(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr);
	virtual ~OWActor() {}

	void getScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required)
	{
		doGetScriptingComponents(ndx, required);
	}
	void scriptor(const OWScriptComponent& newValue) {
		mScriptor = newValue;
	}
	void setup();
	virtual void preTick();
	void tick(float dt);
	virtual void postTick();
	virtual void preRender();
	void render(const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos);

	virtual void postRender()
	{
		doPostRender();
		// Placeholder called on the main thread. OWActor should quickly tidy
		// up whatever prePender() did.
	}

	// Getters/Setters
	void active(bool newValue) {
		mIsActive = newValue;
	}
	bool active() const { return mIsActive; }
	const AABB& bounds() const { return mBounds; }
	void bounds(const AABB& newValue) { mBounds = newValue; }
	Scene* scene() { return mScene; }
	const OWActor* hostActor() const { return mHostActor; }
	std::string name() const { return mName; }
	const OWTransform* transform() const {
		return mActorTransform;
	}
	OWTransform* transform() {
		return mActorTransform;
	}
	void transform(OWTransform* newValue);
	bool setupCompleted() const { return mSetup; }
	void collided(const OWCollider& component, const OWCollider& otherComponent)
	{
		doCollided(component, otherComponent);
	}
	void interpolatePhysics(float totalTime, float alpha, float fixedTimeStep);
#ifdef _DEBUG
	bool debugInclude() const;
#endif
protected:
	virtual void doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required) = 0;
	virtual void copyCurrentToPrevious() = 0;
	virtual void doInterpolatePhysics(float totalTime, float alpha, float fixedTimeStep) = 0;
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) = 0;
	virtual void doSetupActor() = 0;
	virtual void doRender(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& cameraPos) = 0;
	virtual void doPreTick() = 0;
	virtual void doTick(float dt) = 0;
	virtual void doPostTick() = 0;
	virtual void doPreRender() = 0;
	virtual void doPostRender() = 0;
private:
	//std::vector<OWRenderTypes::ActorSetupMutator> mMutatorCallbacks;
	std::string mName;
	AABB mBounds;
	Scene* mScene;
	OWScriptComponent mScriptor;
	OWTransform* mActorTransform = nullptr;
	OWActor* mHostActor = nullptr;
	bool mIsActive = true;
	bool mSetup = false;
};

// Use this class for aggregating discrete Components.
class OWENGINE_API OWActorDiscrete: public OWActor
{
public:
	struct DiscreteEntity
	{
		glm::vec4 colour = { 0,0,0,1.0 };
		OWCollider* coll = nullptr;
		OWPhysics* physics = nullptr;
		OWMeshComponentBase* mesh = nullptr;
		OWRenderer* rend = nullptr;
		OWSoundComponent* sound = nullptr;
	};
	OWActorDiscrete(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr)
		: OWActor(_scene, _name, _hostActor) {
	}
	size_t addComponents(const DiscreteEntity& newElement);
protected:
	virtual void doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required) override final;
	virtual void copyCurrentToPrevious() override final
	{
		for (auto& elm : mElements)
		{
			elm.physics->copyCurrentToPrevious();
		}
	}
	virtual void doInterpolatePhysics(float totalTime, float alpha, float fixedTimeStep) override;
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) override;
	void doSetupActor() override final;
	virtual void doPreTick() override;
	virtual void doTick(float dt) override;
	virtual void doPostTick() override;
	virtual void doPreRender() override;
	virtual void doPostRender() override;
	void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos) override;
	std::vector<DiscreteEntity> mElements;
};

// Use this class for aggregating N distinct meshes that share 
// a common Renderer and Texture
// Use this class to render Text
class OWENGINE_API OWActorNCom1Ren: public OWActor
{
public:
	struct NCom1RenElement
	{
		glm::vec4 colour = { 0,0,0,1.0 };
		OWCollider* coll = nullptr;
		OWMeshComponentBase* mesh = nullptr;
		OWPhysics* physics = nullptr;
		OWSoundComponent* sound = nullptr;
	};
	OWActorNCom1Ren(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr)
		: OWActor(_scene, _name, _hostActor) {
	}
	size_t addComponents(const NCom1RenElement& newElement);
	void renderer(OWRenderer* newValue) { 
		mRenderer = newValue; 
	}
protected:
	virtual void doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required) override final;
	virtual void copyCurrentToPrevious() override final
	{
		for (auto& elm : mElements)
		{
			elm.physics->copyCurrentToPrevious();
		}
	}
	virtual void doInterpolatePhysics(float totalTime, float alpha, float fixedTimeStep) override;
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) override;
	void doSetupActor() override final;
	virtual void doPreTick() override;
	virtual void doTick(float dt) override;
	virtual void doPostTick() override;
	virtual void doPreRender() override;
	virtual void doPostRender() override;
	void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos) override;
private:
	std::vector<NCom1RenElement> mElements;
	OWRenderer* mRenderer = nullptr;
};

// Use this class for aggregating mutable Particles (one Mesh, one Renderer, independant movement)
class OWENGINE_API OWActorMutableParticle: public OWActor
{
public:
	struct MutableParticleElement
	{
		glm::vec4 colour = { 0,0,0,0 };
		OWCollider* coll = nullptr;
		OWPhysics* physics = nullptr;
	};
	OWActorMutableParticle(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr)
		: OWActor(_scene, _name, _hostActor) {
	}
	size_t addComponents(const MutableParticleElement& newElement);
	void renderer(OWRenderer* newValue);
	void sound(OWSoundComponent* newValue);
	void meshComponent(OWMeshComponentBase* mc) {
		mMeshTemplate = mc;
	}

protected:
	virtual void doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required) override final;
	virtual void copyCurrentToPrevious() override final
	{
		for (auto& elm : mElements)
		{
			elm.physics->copyCurrentToPrevious();
		}
	}
	virtual void doInterpolatePhysics(float totalTime, float alpha, float fixedTimeStep) override;
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) override;
	void doSetupActor() override final;
	virtual void doPreTick() override;
	virtual void doTick(float dt) override;
	virtual void doPostTick() override;
	virtual void doPreRender() override;
	virtual void doPostRender() override;
	virtual void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos) override;
private:
	std::vector<MutableParticleElement> mElements;
	OWMeshComponentBase* mMeshTemplate = nullptr;
	OWRenderer* mRenderer = nullptr;
	OWSoundComponent* mSound = nullptr;
};

// Use this class for aggregating immutable Particles 
// (one Mesh, one Renderer, no movement, fixed positions, no interaction with anything)
class OWENGINE_API OWActorImmutableParticle: public OWActor
{
public:
	OWActorImmutableParticle(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr);
	void sound(OWSoundComponent* newValue);
	void renderer(OWRenderer* newValue);
	void meshComponent(OWMeshComponentBase* mc) {
		mMeshTemplate = mc;
	}
protected:
	virtual void doGetScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required) override final;
	virtual void copyCurrentToPrevious() override final
	{
		// Nothing changes, do nothing
	}
	virtual void doInterpolatePhysics(float totalTime, float alpha, float fixedTimeStep) override
	{
		// Nothing changes, do nothing
	}
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) override;
	void doSetupActor() override final;
	virtual void doPreTick() override;
	virtual void doTick(float dt) override;
	virtual void doPostTick() override;
	virtual void doPreRender() override;
	virtual void doPostRender() override;
	void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos) override;
private:
	OWMeshComponentBase* mMeshTemplate = nullptr;
	OWPhysics* mPhysics = nullptr;
	OWRenderer* mRenderer = nullptr;
	OWSoundComponent* mSound = nullptr;
};

/*
struct OWENGINE_API OWRayComponentData
{
	glm::vec4 colour;
	float cooldownTime = 1.0; // cooldown time of beam???
	float elapsedTime = 0;
	bool triggered = false;
};

class OWRayActor : public OWActorSingle
{
	OWRayComponentData mData;
	OWRay* mRay = nullptr;
public:
	OWRayActor(Scene* _scene, const std::string& _name) //Colour??
		: OWActorSingle(_scene, _name) {
	};
	void colour(const glm::vec4& newValue) { mData.colour = newValue; }
	void direction(const glm::vec3& newValue) { 
		glm::vec3 origin = transform()->worldPosition();
		glm::vec3 dir = newValue;
		glm::vec3 tgt = origin + glm::normalize(newValue) * 10000.0f;
		// mElements[0].m->colour() = mData.colour;
		mElements[0].c->points(origin, newValue, tgt);
	}
	bool trigger() { mData.triggered = true; }
	bool intersects(const AABB& box, glm::vec3& normal, float& distance) const;
	bool intersects(const OWCollider* c, glm::vec3& direction, float& distance) const;
protected:
	void doSetup() override
	{
		OWActorSingle::doSetup();
		CollisionSystem::addRay(mElements[0].c, this, 0);
	}
	void render(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4& model,
		const glm::vec3& cameraPos) override
};
*/

