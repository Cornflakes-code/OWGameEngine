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
#include "../Scripting/OWScript.h"

class Scene;

class OWENGINE_API OWActor
{
public:
	OWActor(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr);
	virtual ~OWActor() {}
	virtual void preRender()
	{
		// Placeholder called on the main thread. OWActor should quickly 
		// create a background thread to do stuff while render is happenening.
	}
	virtual void postRender()
	{
		// Placeholder called on the main thread. OWActor should quickly tidy
		// up whatever prePender did.
	}
	const AABB& bounds() const { return mBounds; }
	void bounds(const AABB& newValue) { mBounds = newValue; }
	Scene* scene() { return mScene; }
	const OWActor* hostActor() const { return mHostActor; }
	std::string name() const { return mName; }
	const OWTransform* transform() const
	{
		return mActorTransform;
	}
	OWTransform* transform() 
	{
		return mActorTransform;
	}
	void transform(OWTransform* newValue)
	{
		mActorTransform = newValue;
	}
	void transform(OWTransformData& td) {
		transform(new OWTransform(mActorTransform, td));
	}
	void transform(const glm::vec3& pos, 
			const glm::vec3& scale, const glm::quat& rot = glm::quat()) {
		transform(new OWTransform(mActorTransform, pos, scale, rot));
	}
	void scriptor(OWScriptComponent* newValue) {
		mScriptor = newValue;
	}
	void setup()
	{
		if (!mSetup)
		{
			doSetup();
			mSetup = true;
		}
	}
	void render(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos)
	{
		doRender(proj, view, model, cameraPos);
	}
	bool setupCompleted() const { return mSetup; }
protected:
	virtual void doSetup() = 0;
	virtual void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) = 0;
private:
	std::string mName;
	Scene* mScene;
	OWTransform* mActorTransform = nullptr;
	OWScriptComponent* mScriptor = nullptr;
	bool mIsActive = false;
	bool mSetup = false;
	OWActor* mHostActor = nullptr;
	AABB mBounds;
};

// Use this class for aggregating discrete Components.
class OWENGINE_API OWActorDiscrete: public OWActor
{
public:
	struct DiscreteEntity
	{
		OWCollider* coll = nullptr;
		OWPhysics* phys = nullptr;
		OWMeshComponentBase* mesh = nullptr;
		OWRenderer* rend = nullptr;
		OWTransform* trans = nullptr;
		OWSoundComponent* sound = nullptr;
	};
	OWActorDiscrete(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr)
		: OWActor(_scene, _name, _hostActor) {
	}
	size_t addComponents(const DiscreteEntity& newElement);
protected:
	void doSetup() override;
	void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) override;
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
		glm::vec4 colour = { 0,0,0,0 };
		OWCollider* coll = nullptr;
		OWMeshComponentBase* mesh = nullptr;
		OWPhysics* phys = nullptr;
		OWSoundComponent* sound = nullptr;
		OWTransform* trans = nullptr;
	};
	OWActorNCom1Ren(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr)
		: OWActor(_scene, _name, _hostActor) {
	}
	size_t addComponents(const NCom1RenElement& newElement);
	void renderer(OWRenderer* newValue) { 
		mRenderer = newValue; 
	}
protected:
	void doSetup() override;
	void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) override;
private:
	std::vector<float> mSSBO;
	OWRenderer* mRenderer = nullptr;
	std::vector<NCom1RenElement> mElements;
};

// Use this class for aggregating mutable Particles (one Mesh, one Renderer, independant movement)
class OWENGINE_API OWActorMutableParticle: public OWActor
{
public:
	struct MutableParticleElement
	{
		glm::vec4 colour = { 0,0,0,0 };
		OWCollider* coll = nullptr;
		OWTransform* trans = nullptr;
		OWPhysics* phys = nullptr;
	};
	OWActorMutableParticle(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr)
		: OWActor(_scene, _name, _hostActor) {
	}
	size_t addComponents(const MutableParticleElement& newElement);
	void renderer(OWRenderer* newValue);
	void sound(OWSoundComponent* newValue);
	void meshComponent(OWMeshComponent* mc) {
		mMeshTemplate = mc;
	}

protected:
	void doSetup() override;
	virtual void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) override;
private:
	OWMeshComponent* mMeshTemplate = nullptr;
	OWPhysics* mPhysics = nullptr;
	OWRenderer* mRenderer = nullptr;
	OWSoundComponent* mSound = nullptr;
	std::vector<MutableParticleElement> mElements;
};

class OWInstanceRenderer;
struct InstanceData;
// Use this class for aggregating immutable Particles 
// (one Mesh, one Renderer, no movement, fixed positions, no interaction with anything)
class OWENGINE_API OWActorImmutableParticle: public OWActor
{
public:
	OWActorImmutableParticle(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr);
	void renderer(OWInstanceRenderer* newValue);
	void sound(OWSoundComponent* newValue);
	void instanceMesh(const InstanceData& _data, std::string& _name);
protected:
	void doSetup() override;
	void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) override;
private:
	OWMeshComponent* mMeshTemplate = nullptr;
	OWPhysics* mPhysics = nullptr;
	OWInstanceRenderer* mRenderer = nullptr;
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

