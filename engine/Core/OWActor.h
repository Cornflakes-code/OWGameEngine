#pragma once

#include "OWObject.h"
#include <vector>

#include "../OWEngine/OWEngine.h"

#include "OWComponent.h"
#include "OWMovableComponent.h"
#include "OWSceneComponent.h"
#include "../Renderers/OWRenderable.h"

class Scene;
class OWENGINE_API OWActor: public OWObject, public OWRenderable
{
	Scene* mScene;
	OWMovableComponent mCurrent;
public:
	OWActor(Scene* _scene, const glm::vec3& _position)
		: mScene(_scene), mCurrent(this, _position)
	{}

	std::vector<OWComponent*> mComponents;
	std::vector<OWSceneComponent*> mSceneComponents; // Tree
	enum TickType { InitialTick, FinalTick };
	virtual int tick(float dt, TickType OW_UNUSED(tt = InitialTick))
	{
		for (auto& c : mComponents)
			c->tick(dt);
		for (auto& c : mSceneComponents)
			c->tick(dt);
		return 0;
	}
	virtual void addComponent(OWComponent* c) { mComponents.push_back(c); }
	virtual void addSceneComponent(OWSceneComponent* c) { mSceneComponents.push_back(c); }
	void traverse(OWSceneComponent::OWSceneComponentCallbackType cb)
	{
		for (OWSceneComponent* sc : mSceneComponents)
		{
			cb(sc);
		}
	}
	const glm::vec3& position() const { return mCurrent.position(); }
	bool collideHandled(OWMovableComponent* OW_UNUSED(_ourComponent), OWActor* OW_UNUSED(other), OWMovableComponent* OW_UNUSED(otherComponent))
	{
		// returning true means we have dealt with it
		// returning false lets _ourComponent deal with it. I assume they just rebound.
		return false;
	}
	virtual bool canCollide(OWMovableComponent* OW_UNUSED(_ourComponent), OWActor* OW_UNUSED(other), OWMovableComponent* OW_UNUSED(otherComponent))
	{
		// for example our thigh and shin of same leg may interesect but they cannot collide
		// but hands of different arms can.
		return true;
	}

	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) const override;
};
