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
	std::string mName;
	Scene* mScene;
	OWTransform* mActorTransform = nullptr;
	OWScriptComponent* mScriptor = nullptr;
	bool mIsActive = false;
	bool mSetup = false;
	OWActor* mHostActor = nullptr;
public:
	OWActor(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr);
	virtual ~OWActor() {}
	Scene* scene() { return mScene; }
	const OWActor* hostActor() const { return mHostActor; }
	std::string name() const { return mName; }
	OWTransform* transform() {
		return mActorTransform;
	}
	void transform(OWTransform* newValue) {
		newValue->hostingTransform(mActorTransform);
		mActorTransform = newValue;
	}
	void transform(const OWTransformData& td) {
		transform(new OWTransform(mActorTransform, td));
	}
	void transform(const glm::vec3& pos, 
			const glm::vec3& scale, const glm::quat& rot = glm::quat()) {
		transform(new OWTransform(mActorTransform, pos, scale, rot));
	}
	const OWTransform* transform() const {
		return mActorTransform; 
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
protected:
	virtual void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) = 0;
	virtual void doSetup() = 0;
};

// Use this class for aggregating Components that each have different mesh and Renderers.
class OWENGINE_API OWActorSingle: public OWActor
{
public:
	struct SingleSceneElement
	{
		OWCollider* c = nullptr;
		OWPhysics* p = nullptr;
		OWMeshComponentBase* m = nullptr;
		OWRenderer* r = nullptr;
		OWTransform* t = nullptr;
		OWSoundComponent* s = nullptr;
	};
	OWActorSingle(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr)
		: OWActor(_scene, _name, _hostActor) {
	}
	size_t addComponents(const SingleSceneElement& sse);
protected:
	void ensureActor(OWComponent* c)
	{
		if (c->actor() == nullptr)
			c->actor(this);
	}
	virtual void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) override;
	void doSetup() override;
	std::vector<SingleSceneElement> mElements;
};

// Use this class for Components that use the same Renderer
class OWENGINE_API OWActorMulti : public OWActor
{
public:
	struct MultiSceneElement
	{
		// We could add a OWMeshComponent* here with the logic being
		// getComponent[i] 
		// { if mElements[i] is nullptr then
		//		return mMeshComponentTemplate
		// This would allow all the null OWMeshComponent to use the mesh
		// in mMeshComponentTemplate.

		glm::vec4 colour = { 0,0,0,0 };
		OWCollider* c = nullptr;
		OWTransform* t = nullptr;
		OWPhysics* p = nullptr;
		// If OWMeshComponentBase is nullptr then use the mesh in mMeshComponentTemplate
		//OWMeshComponentBase* m = nullptr;
	};
	OWActorMulti(Scene* _scene, const std::string& _name, OWActor* _hostActor = nullptr)
		: OWActor(_scene, _name, _hostActor) {
	}
	void addComponents(const MultiSceneElement& toAdd);
	void physics(OWPhysics* newValue);
	void renderer(OWRenderer* newValue);
	void sound(OWSoundComponent* newValue);
	void meshComponent(OWMeshComponent* mc) {
		mMeshComponentTemplate = mc;
	}

protected:
	virtual void doRender(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) override;
	void doSetup();
private:
	OWMeshComponent* mMeshComponentTemplate = nullptr;
	OWPhysics* mPhysics = nullptr;
	OWRenderer* mRenderer = nullptr;
	OWSoundComponent* mSound = nullptr;
	bool mIsActive = false;
	std::vector<MultiSceneElement> mElements;
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

