#pragma once

#include <glm/gtx/quaternion.hpp>

#include "../OWEngine/OWEngine.h"
#include "OWMovableComponent.h"
#include "../Renderers/OWRenderable.h"
#include "../Renderers/RendererBase.h"

struct OWENGINE_API OWSceneComponentData: public OWMoveData

{
	glm::vec3 scale = glm::vec3(1);
};

class OWENGINE_API OWSceneComponent: public OWComponent, public OWIMovable, public OWIRenderable
{
protected:
	RendererBase* mRenderer = nullptr;
	virtual OWSceneComponentData* data() override
	{
		return static_cast<OWSceneComponentData*>(data());
	}
	void addRenderer(RendererBase* r) { mRenderer = r; }
public:
	OWSceneComponent(OWActor* _owner, OWSceneComponentData* _data = nullptr);
	virtual const OWSceneComponentData* constData() const override
	{
		return static_cast<const OWSceneComponentData*>(constData());
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
