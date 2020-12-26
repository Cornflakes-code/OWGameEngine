#include "Mesh.h"

#include <glm/ext/matrix_transform.hpp>

#include "../Helpers/MoveController.h"

#include "MeshRenderer.h"
#include "../Helpers/ModelFactory.h"

void Mesh::readFiles(const std::string& modelFileName,
				const std::string& textureFileName, bool cache)
{
	mData = ModelFactory().create(modelFileName, textureFileName, cache);
}

void Mesh::makeDeep()
{
	if (mData.use_count() == 0)
	{
		// Unititialised object?
	}
	else if (mData.use_count() == 1)
	{
		// The original object or we already have a deep copy
	}
	if (mData.use_count() > 1)
	{
		// We currently have a shallow_copy
		ModelData* originalData = mData.get();
		ModelData* md = new ModelData(*originalData);
		mData = std::shared_ptr<ModelData>(md);
	}
}

void Mesh::init(const std::vector<Mesh::Vertex>& vertices,
	const std::vector<unsigned int>& indices, bool _calcNormals)
{
	if (mData.use_count() != 0)
	{
		throw NMSLogicException("Trying to init a previously initialised model.");
	}
	ModelData* md = new ModelData();
	md->vertices = vertices;
	md->indices = indices;
	if (_calcNormals)
	{
		calcNormals(md);
	}
}

void Mesh::calcNormals(ModelData* md) const
{
	for (int i = 0; i < md->indices.size(); i += 3)
	{
		int i0 = md->indices[i];
		int i1 = md->indices[i + 1];
		int i2 = md->indices[i + 2];

		glm::vec3 v1 = md->vertices[i1].position - md->vertices[i0].position;
		glm::vec3 v2 = md->vertices[i2].position - md->vertices[i0].position;
		glm::vec3 v2Normal = glm::normalize(v2);
		glm::vec3  normal = glm::cross(v1, v2Normal);

		md->vertices[i0].normal += normal;
		md->vertices[i1].normal += normal;
		md->vertices[i2].normal += normal;
	}

	for (int i = 0; i < md->vertices.size(); i++)
		md->vertices[i].normal = glm::normalize(md->vertices[i].normal);
}

void Mesh::addRenderer(ModelRenderer* source)
{
	mRenderer = source;
	mRenderer->prepare(this);
}

AABB Mesh::calcBounds() const
{
	return AABB();
}

void Mesh::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const MoveController* mover,
	OWUtils::RenderCallbackType renderCb,
	OWUtils::ResizeCallbackType resizeCb) const
{
	if (mover)
	{
		mRenderer->render(this, proj, view, mover->translate(model), renderCb, resizeCb);
	}
	else
	{
		glm::mat4 initialPositionModel = glm::translate(model, initialPosition());
		mRenderer->render(this, proj, view, initialPositionModel, renderCb, resizeCb);
	}
}
