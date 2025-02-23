#pragma once

#include <vector>


#include "../OWEngine/OWEngine.h"
#include "OWActor.h"
#include "../Component/OWComponent.h"
#include "../Component/PhysicalComponent.h"
#include "../Component/OWSceneComponent.h"
#include "../Renderers/OWRenderable.h"

class Scene;
struct OWENGINE_API BB3DRendererData
{
};

class OWENGINE_API BB3DRenderer: public OLDObject, public OLDGameIFace, public OLDIRenderable
{
	std::vector<OLDSceneComponent*> m2DComponents;
	std::vector<OLDSceneComponent*> m3DComponents;
	BB3DRendererData* mData;
public:
	BB3DRenderer(Scene* _scene, BB3DRendererData* _data)
	{
	}
	void add2DBoundingBox(OLDSceneComponent* o);
	void add3DBoundingBox(OLDSceneComponent* o);
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
