#include "ModelRenderer.h"

#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/ErrorHandling.h"
#include "../Helpers/CommonUtils.h"
#include "../Helpers/ModelData.h"
#include "../Core/GlobalSettings.h"

#include "Model.h"
#include "Mesh.h"
#include "MeshRenderer.h"

ModelRenderer::ModelRenderer()
{
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::prepare(const Model* source)
{
	source->mRootNode->dfsTraverse([source](ModelData* m) {
		for (auto& mesh : m->meshes)
		{
			mesh.shader(source->shader(), source->pvmName());
			mesh.renderer(new MeshRenderer());
		}
	});
	checkSourceForErrors(source);
}

void ModelRenderer::render(const Model* source,
	const glm::mat4& proj,
	const glm::mat4& view, 
	const glm::mat4& model,
	OWUtils::RenderCallbackType renderCb,
	OWUtils::ResizeCallbackType resizeCb) const
{
	OWUtils::PolygonModeRIAA poly;

	if (!renderCb)
	{
		// renderCb = code that moves applies pvm to children in source->mRootNode
	}
	if (!resizeCb)
	{
		// resize all in source->mRootNode
	}
	source->mRootNode->dfsTraverse([&](ModelData* m) {
		for (auto& mesh : m->meshes)
			mesh.render(proj, view, model, nullptr, renderCb, resizeCb);
	});
}

void ModelRenderer::checkSourceForErrors(const Model* source) const
{
	checkRenderBaseForErrors(source);
	if (source->mRenderer == nullptr)
		throw NMSLogicException("source->mRenderer must not be null");
	if (!source->mRootNode)
		throw NMSLogicException("model has no mesh");
	// call checkSourceForErrors on every Mesh
	source->mRootNode->dfsTraverse([source](ModelData* m) {
		for (auto& mesh : m->meshes)
		{
			mesh.renderer()->checkSourceForErrors(&mesh);
		}
	});
}