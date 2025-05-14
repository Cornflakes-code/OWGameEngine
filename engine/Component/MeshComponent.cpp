#include "MeshComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/Shader.h"
#include "../Actor/OWActor.h"

/*
* Very Modern OpenGL
https://www.reddit.com/r/opengl/comments/18rkgg3/one_vao_for_multiple_vbos/
https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions#glbuffer
https://fgiesen.wordpress.com/2011/07/01/a-trip-through-the-graphics-pipeline-2011-part-1/
*/

OWMeshComponent::OWMeshComponent(OWActor* _owner, const std::string& _name)
	: OWMeshComponentBase(_owner, _name, OWRenderTypes::DrawType::ThreeD)
{
}

OWMeshComponent* OWMeshComponent::setData(const MeshData& mesh)
{
	if (this->actor()->setupCompleted())
	{
		if (mData.v4.size() && (mData.v4.size() != mesh.v4.size()))
			throw NMSLogicException("OWMeshComponent::replaceData mesh sizes must be the same.");
		if (mData.indices.size() && (mData.indices.size() != mesh.indices.size()))
			throw NMSLogicException("OWMeshComponent::replaceData indices sizes must be the same.");

		this->actor()->preModifyMesh(this);
	}
	mData = mesh;
	if (this->actor()->setupCompleted())
	{
		mData.updateMeshOnly(true);
		// We are updating the mesh after a prior setup
		this->actor()->postModifyMesh(this);
		mData.updateMeshOnly(false);
	}
	return this;
}

const OWRenderData OWMeshComponent::renderData(AABB& bounds)
{
	bounds = mData.bounds();
	OWRenderData rd;
	rd.meshes.push_back(mData);
	if (!retainMesh())
	{
		mData = MeshData();
	}
	return rd;
}

void OWMeshComponent::doSetup()
{
	validate(mData);
}
