#pragma once

#include "../OWEngine/OWEngine.h"
#include "OWComponent.h"
#include "../Renderers/OWRenderable.h"

class OWActor;
class RendererBase;
class OWENGINE_API OWRenderableComponent: public OWIRenderable
{
	RendererBase* mRenderer = nullptr;
	bool mRenderThis = true;
	bool mReadyForRender = false;
protected:
	OWActor* mOwner;
	void readyForRender(bool newValue) { mReadyForRender = newValue; }
public:
	bool readyForRender() const { return mReadyForRender; }
	OWRenderableComponent(OWActor* _owner)
		: OWComponent(_owner) {
	}
	virtual int preTick() = 0;
	virtual int tick(float dt) = 0;
	virtual int postTick() = 0;
	void canRender(bool newValue) { mRenderThis = newValue; }
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) override;
};
