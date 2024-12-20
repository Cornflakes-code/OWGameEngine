#include "OWSceneComponent.h"

#include "OWActor.h"

OWSceneComponent::OWSceneComponent(OWActor* _owner, const glm::vec3& _position)
	: OWMovableComponent(_owner, _position) 
{
	_owner->addSceneComponent(this);
}


void OWSceneComponent::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const glm::vec3& cameraPos,
	RenderTypes::ShaderMutator renderCb,
	RenderTypes::ShaderResizer resizeCb) const 
{
	std::string s = name();
	if (s == "stars")
		s = "stars";
	if (mRenderThis)
	{
		glm::mat4 _model = model;
		for (ModelModifierCallbackType cb : mModelChangers)
		{
			_model = cb(_model);
		}		
		mRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);
	}
}
