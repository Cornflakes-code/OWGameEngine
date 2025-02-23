#pragma once

#include <glm/gtx/quaternion.hpp>

#include "../OWEngine/OWEngine.h"
#include "PhysicalComponent.h"
#include "../Renderers/OWRenderable.h"
#include "../Renderers/RendererBase.h"
#include "OWComponent.h"

struct OWENGINE_API OLDSceneComponentData: public OWPhysicsData
{
	std::string name;
};

class OWENGINE_API OLDSceneComponent: public OLDComponent, public OLDIPhysical, public OLDIRenderable
{
protected:
	RendererBase* mBoundingBoxRenderer = nullptr;
	AABB mOriginalBoundingBox;
	RendererBase* mRenderer = nullptr;
	virtual OLDSceneComponentData* data() override
	{
		return static_cast<OLDSceneComponentData*>(OLDIPhysical::data());
	}
	void addRenderer(RendererBase* r) { mRenderer = r; }
public:
	typedef std::function<void(OLDSceneComponent* sc)> OWSceneComponentCallbackType;
	OLDSceneComponent(OLDActor* _owner, OLDSceneComponentData* _data = nullptr);
	bool canCollide() override;
	bool canCollide(OLDCollisionData* other) override;
	void collided(OLDCollisionData* other) override;
	bool collides(OLDCollisionData* other) override;
	void doInit() override;

	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) override;

};
