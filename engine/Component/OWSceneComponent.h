#pragma once

#include <glm/gtx/quaternion.hpp>

#include "../OWEngine/OWEngine.h"
#include "PhysicalComponent.h"
#include "../Renderers/OWRenderable.h"
#include "../Renderers/RendererBase.h"
#include "OWComponent.h"

struct OWENGINE_API OWSceneComponentData: public OWPhysicsData
{
	std::string name;
};

class OWENGINE_API OWSceneComponent: public OWComponent, public OWIPhysical, public OWIRenderable
{
protected:
	RendererBase* mBoundingBoxRenderer = nullptr;
	AABB mOriginalBoundingBox;
	RendererBase* mRenderer = nullptr;
	virtual OWSceneComponentData* data() override
	{
		return static_cast<OWSceneComponentData*>(OWIPhysical::data());
	}
	void addRenderer(RendererBase* r) { mRenderer = r; }
public:
	typedef std::function<void(OWSceneComponent* sc)> OWSceneComponentCallbackType;
	OWSceneComponent(OWActor* _owner, OWSceneComponentData* _data = nullptr);
	bool canCollide() override;
	bool canCollide(OWCollisionData* other) override;
	void collided(OWCollisionData* other) override;
	bool collides(OWCollisionData* other) override;
	void doInit() override;

	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) override;

};
