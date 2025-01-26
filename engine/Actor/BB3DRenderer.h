#pragma once

#include <vector>


#include "../OWEngine/OWEngine.h"
#include "OWActor.h"
#include "../Component/OWComponent.h"
#include "../Component/OWMovableComponent.h"
#include "../Component/OWSceneComponent.h"
#include "../Renderers/OWRenderable.h"

class Scene;
struct OWENGINE_API BB3DRendererData
{
};

class OWENGINE_API BB3DRenderer: public OWObject, public OWGameIFace, public OWIRenderable
{
	std::vector<OWSceneComponent*> m2DComponents;
	std::vector<OWSceneComponent*> m3DComponents;
	BB3DRendererData* mData;
public:
	BB3DRenderer(Scene* _scene, BB3DRendererData* _data)
	{
	}
	void add2DBoundingBox(OWSceneComponent* o);
	void add3DBoundingBox(OWSceneComponent* o);
	int preTick();// override;
	void tick(float dt);// override;
	int postTick();// override;
	virtual void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr) override;
};
