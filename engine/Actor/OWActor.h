#pragma once

#include "../Core/OWObject.h"
#include <vector>

#include "../OWEngine/OWEngine.h"
#include "../Scripting/OWActorScript.h"
#include "../Component/OWComponent.h"
#include "../Component/PhysicalComponent.h"
#include "../Component/OWSceneComponent.h"
#include "../Renderers/OWRenderable.h"
#include "../Helpers/Transform.h"

class OWMeshComponent;
class OWSoundComponent;
class OWCollider;
class OWScriptComponent;
class OWPhysics;
class Scene;
class OWRenderer;

class OWActor
{
	std::string mName;
	Scene* mScene;
	OWTransform* mActorTransform;
	OWScriptComponent* mScriptor;
	bool mIsActive = false;
	bool mSetup = false;
public:
	OWActor(Scene* _scene, const std::string& _name);
	virtual ~OWActor() {}
	OWTransform* transform() {
		return mActorTransform;
	}
	void transform(OWTransform* newValue) {
		newValue->actor(this);
		mActorTransform = newValue;
	}
	void transform(const OWTransformData& td) {
		transform(new OWTransform(this, td));
	}
	void transform(const glm::vec3& pos, 
			const glm::vec3& scale, const glm::quat& rot = glm::quat()) {
		transform(new OWTransform(this, pos, scale, rot));
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
protected:
	virtual void doSetup() = 0;
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model, 
		const glm::vec3& cameraPos) = 0;
};

class OWActorSingle: public OWActor
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
	OWActorSingle(Scene* _scene, const std::string& _name)
		: OWActor(_scene, _name) {
	}
	void setComponent(const SingleSceneElement& sse);
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) override;
protected:
	void doSetup() override;
	std::vector<SingleSceneElement> mElements;
};

struct OWENGINE_API OWRayComponentData
{
	glm::vec4 colour;
	float cooldownTime = 1.0; // cooldown time of beam???
	float elapsedTime = 0;
	bool triggered = false;
};
/*
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

class OWActorMulti: public OWActor
{
public:
	struct MultiSceneElement
	{
		glm::vec4 colour = { 0,0,0,0 };
		OWCollider* c = nullptr;
		OWTransform* t = nullptr;
		OWPhysics* p = nullptr;
	};
	OWActorMulti(Scene* _scene, const std::string& _name)
		: OWActor(_scene, _name) {
	}
	void addElement(const MultiSceneElement& toAdd);
	void physics(OWPhysics* newValue);
	void renderer(OWRenderer* newValue);
	void sound(OWSoundComponent* newValue);
	void meshComponent(OWMeshComponent* mc) {
		mMeshComponentTemplate = mc;
	}

	virtual void render(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4 model,
		const glm::vec3& cameraPos) override;
protected:
	void doSetup();
private:
	OWMeshComponent* mMeshComponentTemplate;
	OWPhysics* mPhysics = nullptr;
	OWRenderer* mRenderer = nullptr;
	OWSoundComponent* mSound = nullptr;
	bool mIsActive = false;
	std::vector<MultiSceneElement> mElements;
};
