#pragma once

#include "../OWEngine/OWEngine.h"
#include "OWMovableComponent.h"
#include "../Renderers/OWRenderable.h"
#include "../Renderers/RendererBase.h"

class OWENGINE_API OWSceneComponent : public OWMovableComponent, public OWRenderable
{
	// ModelModifierCallbackType propably should be replaced with a quaternian.
	//typedef std::function<glm::mat4(const glm::mat4& model)> ModelModifierCallbackType;
	//std::vector< ModelModifierCallbackType> mModelChangers;
protected:
	RendererBase* mRenderer = nullptr;
	bool mRenderThis = true;
	bool mReadyForRender = false;
	glm::mat4 mModelMatrix = glm::mat4(1.0f);
	glm::vec3 mScale = glm::vec3(1);
	glm::vec3 mRotateAxis = glm::vec3(1);
	float mRotateRadians = 0.0f;
protected:

	void readyForRender(bool newValue) { mReadyForRender = newValue; }
public:
	bool readyForRender() const { return mReadyForRender; }
	OWSceneComponent(OWActor* _owner, const glm::vec3& _position);
	typedef std::function<void(OWSceneComponent* sc)> OWSceneComponentCallbackType;
	void addRenderer(RendererBase* r) { mRenderer = r; }
	void scale(const glm::vec3& factor);
	glm::vec3 scale() const { return mScale; }
	void rotate(float degrees, const glm::vec3& axis);
	//void addModelModifier(ModelModifierCallbackType cb) { mModelChangers.push_back(cb); }
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
		RenderTypes::ShaderResizer resizeCb = nullptr) override;
};
