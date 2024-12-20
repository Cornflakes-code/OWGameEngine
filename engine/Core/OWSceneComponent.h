#pragma once

#include "../OWEngine/OWEngine.h"
#include "OWMovableComponent.h"
#include "../Renderers/OWRenderable.h"
#include "../Renderers/RendererBase.h"

class OWENGINE_API OWSceneComponent : public OWMovableComponent, public OWRenderable
{
	// ModelModifierCallbackType propably should be replaced with a quaternian.
	typedef std::function<glm::mat4(const glm::mat4& model)> ModelModifierCallbackType;
	std::vector< ModelModifierCallbackType> mModelChangers;
	RendererBase* mRenderer = nullptr;
	bool mRenderThis = true;
public:
	OWSceneComponent(OWActor* _owner, const glm::vec3& _position);

	typedef std::function<void(OWSceneComponent* sc)> OWSceneComponentCallbackType;
	void addRenderer(RendererBase* r) { mRenderer = r; }
	void addModelModifier(ModelModifierCallbackType cb) { mModelChangers.push_back(cb); }
	int tick(float dt) override
	{
		return OWMovableComponent::tick(dt);
	}
	void canRender(bool newValue) { mRenderThis = newValue; }
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) const override;
};
