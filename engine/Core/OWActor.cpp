#include "OWActor.h"


void OWActor::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4& _model,
	const glm::vec3& cameraPos,
	RenderTypes::ShaderMutator renderCb,
	RenderTypes::ShaderResizer resizeCb)
{
	std::string s = name();
	if (s == "stars")
		s = "stars";
	if (s == "Text: Enjoy it while you can")
	{
		std::string s = "Text: Enjoy it while you can";
	}
	for (OWSceneComponent* c : mSceneComponents)
	{
		c->render(proj, view, _model, cameraPos,
			renderCb, resizeCb);
	}
}
