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
class OLDSceneComponent;
class OWENGINE_API OLDActor: public OLDObject, public OLDGameIFace, public OLDIRenderable
{
	Scene* mScene;
	OLDActorScript* mScript;
	std::vector<OLDSceneComponent*> mSceneComponents;
protected:
	virtual OLDActorScript* script()
	{
		return mScript;
	}
	void doInit() override;
public:
	OLDActor(Scene* _scene, OLDActorScript* _script);
	const OLDActorData* data() const
	{
		return mScript->data();
	}
	virtual const OLDActorScript* script() const
	{
		return mScript;
	}
	virtual void addSceneComponent(OLDSceneComponent* c) { mSceneComponents.push_back(c); }
	bool collideHandled(OLDIPhysical* OW_UNUSED(_ourComponent), OLDActor* OW_UNUSED(other), OLDIPhysical* OW_UNUSED(otherComponent))
	{
		// returning true means we have dealt with it
		// returning false lets _ourComponent deal with it. I assume they just rebound.
		return false;
	}
	virtual bool canCollide(OLDIPhysical* OW_UNUSED(_ourComponent), OLDActor* OW_UNUSED(other), OLDIPhysical* OW_UNUSED(otherComponent))
	{
		// for example our thigh and shin of same leg may interesect but they cannot collide
		// but hands of different arms can.
		return true;
	}
	void traverse(OLDSceneComponent::OWSceneComponentCallbackType cb)
	{
		for (OLDSceneComponent* sc : mSceneComponents)
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
