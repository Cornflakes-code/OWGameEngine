#pragma once

#include <glm/gtx/quaternion.hpp>

#include "../OWEngine/OWEngine.h"
#include "PhysicalComponent.h"
#include "../Renderers/OWRenderable.h"
#include "../Renderers/RendererBase.h"
#include "OWComponent.h"

struct OWENGINE_API OWSceneComponentData: public OWPhysicsData

{
	glm::vec3 scale = glm::vec3(1);
	std::string name;
};

class OWENGINE_API OWSceneComponent: public OWComponent, public OWIPhysical, public OWIRenderable
{
protected:
	RendererBase* mRenderer = nullptr;
	virtual OWSceneComponentData* data() override
	{
		return static_cast<OWSceneComponentData*>(OWIPhysical::data());
	}
	void addRenderer(RendererBase* r) { mRenderer = r; }
public:
	typedef std::function<void(OWSceneComponent* sc)> OWSceneComponentCallbackType;
	OWSceneComponent(OWActor* _owner, OWSceneComponentData* _data = nullptr);
	virtual const OWSceneComponentData* constData() const override
	{
		return static_cast<const OWSceneComponentData*>(OWIPhysical::constData());
	}
	float visibility() const
	{
		return constData()->physics.visibility;
	}
	void visibility(float newValue) 
	{
		if (newValue < 0.0f)
			newValue = 0.0f;
		else if (newValue > 1.0f)
			newValue = 1.0f;
		data()->physics.visibility = newValue;
	}
	void scale(const glm::vec3& factor);
	glm::vec3 scale() const { return constData()->scale; }
	bool canCollide() override;
	bool canCollide(OWCollisionData* other) override;
	void collided(OWCollisionData* other) override;
	bool collides(OWCollisionData* other) override;

	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) override;

};
