#include "OWActor.h"
#include "../Core/Scene.h"

OLDActor::OLDActor(Scene* _scene, OLDActorScript* _script)
	: mScene(_scene), mScript(_script)
{
	_scene->addActor(this);
}

void OLDActor::doInit()
{
	for (auto& a : mSceneComponents)
	{
		a->init();
	}	
}

void OLDActor::begin()
{
}

void OLDActor::tick(float deltaSecods)
{
}

void OLDActor::end()
{
}

void OLDActor::destroy()
{
}


void OLDActor::render(const glm::mat4& proj,
	const glm::mat4& view, const glm::mat4& _model,
	const glm::vec3& cameraPos,
	RenderTypes::ShaderMutator renderCb,
	RenderTypes::ShaderResizer resizeCb)
{
	std::string s = name();
	if (s == "stars")
		s = "stars";
	if (s == "Text: Enjoy it while you can.")
	{
		s = "Text: Enjoy it while you can.";
	}
	for (OLDSceneComponent* c : mSceneComponents)
	{
		if (c->initCalled())
		{
			c->render(proj, view, _model, cameraPos,
				renderCb, resizeCb);
		}
	}
}
