#pragma once

#include "../Core/OWObject.h"
#include <vector>

#include "../OWEngine/OWEngine.h"
#include "../Scripting/OWActorScript.h"
#include "../Component/OWComponent.h"
#include "../Component/PhysicalComponent.h"
#include "../Component/OWSceneComponent.h"
#include "../Renderers/OWRenderable.h"

class Scene;
class OWSceneComponent;
class OWENGINE_API OWActor: public OWObject, public OWGameIFace, public OWIRenderable
{
	Scene* mScene;
	OWActorScript* mScript;
	std::vector<OWSceneComponent*> mSceneComponents;
protected:
	virtual OWActorScript* script()
	{
		return mScript;
	}
	void doInit() override;
public:
	OWActor(Scene* _scene, OWActorScript* _script);
	const OWActorData* data() const
	{
		return mScript->data();
	}
	virtual const OWActorScript* script() const
	{
		return mScript;
	}
	virtual void addSceneComponent(OWSceneComponent* c) { mSceneComponents.push_back(c); }
	bool collideHandled(OWIPhysical* OW_UNUSED(_ourComponent), OWActor* OW_UNUSED(other), OWIPhysical* OW_UNUSED(otherComponent))
	{
		// returning true means we have dealt with it
		// returning false lets _ourComponent deal with it. I assume they just rebound.
		return false;
	}
	virtual bool canCollide(OWIPhysical* OW_UNUSED(_ourComponent), OWActor* OW_UNUSED(other), OWIPhysical* OW_UNUSED(otherComponent))
	{
		// for example our thigh and shin of same leg may interesect but they cannot collide
		// but hands of different arms can.
		return true;
	}
	void traverse(OWSceneComponent::OWSceneComponentCallbackType cb)
	{
		for (OWSceneComponent* sc : mSceneComponents)
		{
			cb(sc);
		}
	}
protected:
public:
	void begin() override;
	void tick(float deltaSecods) override;
	void end() override;
	void destroy() override;

	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) override;
};
