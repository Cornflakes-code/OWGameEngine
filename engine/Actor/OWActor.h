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

	void setup();
	virtual void preTick();
	void tick(float dt);
	virtual void postTick();
	virtual void preRender();
	void render(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& cameraPos);
	void postRender();

	// Getters/Setters
	void active(bool newValue) {
		mIsActive = newValue;
	}
	bool active() const { 
		return mIsActive; 
	}
	const AABB& bounds() const { 
		return mBounds; 
	}
	void bounds(const AABB& newValue) { 
		mBounds = newValue; 
	}
	void scriptor(const OWScriptComponent& newValue) {
		mScriptor = newValue;
	}
	Scene* scene() { 
		return mScene; 
	}
	const OWActor* hostActor() const { 
		return mHostActor; 
	}
	std::string name() const { 
		return mName; 
	}
	const OWTransform* transform() const {
		return mActorTransform;
	}
	OWTransform* transform() {
		return mActorTransform;
	}
	void transform(OWTransform* newValue);
	bool setupCompleted() const { 
		return mSetup; 
	}
	void collided(const OWCollider& component, const OWCollider& otherComponent);
	void interpolatePhysics(float totalTime, float alpha, float fixedTimeStep);
#ifdef _DEBUG
	bool debugInclude() const;
#endif
	void addRenderer(OWRenderer* rend);
	void addSound(OWSoundComponent* sound);
	void addMeshComponent(OWMeshComponentBase* mesh);
	void getScriptingComponents(int ndx, OWScriptComponent::RequiredComponents& required);
	void setMeshComponent(OWMeshComponentBase* mesh, OWSize ndx);
	OWPhysics* getPhysics(OWSize ndx);
	OWMeshComponentBase* getMeshComponent(OWSize ndx);
	void preModifyMesh(OWMeshComponent* existingMesh);
	void postModifyMesh(OWMeshComponent* modifiedMesh);

protected:
	// Component accessors
	const glm::vec4& getColour(OWSize ndx);
	void setColour(const glm::vec4& colour, OWSize ndx);
	void addColour(const glm::vec4& colour);

	OWCollider* getCollider(OWSize ndx);
	void setCollider(OWCollider* coll, OWSize ndx);
	void addCollider(OWCollider* coll);

	void setPhysics(OWPhysics* phys, OWSize ndx);
	void addPhysics(OWPhysics* phys);

	OWRenderer* getRenderer(OWSize ndx);
	void setRenderer(OWRenderer* rend, OWSize ndx);
	
	OWSoundComponent* getSound(OWSize ndx);
	void setSound(OWSoundComponent* sound, OWSize ndx);

	void copyCurrentToPrevious();
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) = 0;
	virtual void doSetupActor(OWMeshComponentBase* target) = 0;
	virtual void doPreTick() {}
	virtual void doTick(float OW_UNUSED(dt)) {}
	virtual void doPostTick() {}
	virtual void doPreRender() {}
	virtual void doPostRender() {}

	// Array sizes.
	OWSize coloursSize() const { return static_cast<OWSize>(mColours.size()); }
	OWSize collidersSize() const { return static_cast<OWSize>(mColliders.size()); }
	OWSize physicsSize() const { return static_cast<OWSize>(mPhysics.size()); }
	OWSize meshesSize() const { return static_cast<OWSize>(mMeshes.size()); }
	OWSize renderersSize() const { return static_cast<OWSize>(mRenderers.size()); }
	OWSize soundsSize() const { return static_cast<OWSize>(mSounds.size()); }
private:
	std::vector<glm::vec4> mColours;
	std::vector<OWCollider*> mColliders;
	std::vector<OWPhysics*> mPhysics;
	std::vector<OWMeshComponentBase*> mMeshes;
	std::vector<OWRenderer*> mRenderers;
	std::vector<OWSoundComponent*> mSounds;

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
	OWSize addComponents(const DiscreteEntity& newElement);
protected:
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) override;
	void doSetupActor(OWMeshComponentBase* target) override final;
};

// Use this class for aggregating meshes that share a common Renderer and Texture
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
	OWSize addComponents(const NCom1RenElement& newElement);
protected:
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) override;
	void doSetupActor(OWMeshComponentBase* target) override final;
private:
};

// Use this class for aggregating mutable, Identical Particles.
// one Mesh, one Renderer, one Sound, independant movement and colour)
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
	OWSize addComponents(const MutableParticleElement& newElement);

protected:
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) override;
	void doSetupActor(OWMeshComponentBase* target) override final;
private:
};

// Use this class for aggregating immutable Particles 
// (one Mesh, one Renderer, one sound, no movement, fixed positions, no interaction with anything)
class OWENGINE_API OWActorImmutableParticle: public OWActor
{
public:
	struct ImmutableParticleElement
	{
		glm::vec4 colour = { 0,0,0,0 };
		OWRenderer* rend = nullptr;
		OWMeshComponent* mesh = nullptr;
		OWSoundComponent* sound = nullptr;
	};

	OWActorImmutableParticle(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr);
	OWSize addComponents(const ImmutableParticleElement& newElement);
protected:
	virtual void doCollided(const OWCollider& component, const OWCollider& otherComponent) override;
	void doSetupActor(OWMeshComponentBase* target) override final;
private:
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

