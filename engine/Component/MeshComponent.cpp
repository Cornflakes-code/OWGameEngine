#include "MeshComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/CommonUtils.h"
#include "../Core/ErrorHandling.h"

#include "../Helpers/Shader.h"


/*
* Very Modern OpenGL
https://www.reddit.com/r/opengl/comments/18rkgg3/one_vao_for_multiple_vbos/
https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions#glbuffer
https://fgiesen.wordpress.com/2011/07/01/a-trip-through-the-graphics-pipeline-2011-part-1/
*/

OWMeshComponent::OWMeshComponent(OWActor* _owner, const std::string& _name)
	: OWMeshComponentBase(_owner, _name)
{
}

OWMeshComponent* OWMeshComponent::add(const MeshData& meshData)
{
	mData.meshes.push_back(meshData);
	return this;
}

OWMeshComponent* OWMeshComponent::add(const std::vector<glm::vec3>& v)
{
	MeshData mdl;
	mdl.v3 = v;
	add(mdl);
	return this;
}

OWMeshComponent* OWMeshComponent::add(const std::vector<glm::vec4>& v)
{
	MeshData mdl;
	mdl.v4 = v;
	add(mdl);
	return this;
}

const OWRenderData OWMeshComponent::renderData(AABB& bounds) const
{
	bounds = mData.bounds();
	return mData;
}

void OWMeshComponent::doSetup()
{
	for (int i = 0; i < mData.meshes.size(); i++)
	{
		validate(mData.meshes[i]);
	}
}
