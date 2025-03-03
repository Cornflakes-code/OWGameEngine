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

void OWMeshComponent::add(const MeshData& meshData)
{
	validate(meshData);
	OWMeshData md;
	md.meshData = meshData;
	mData.push_back(md);
}

void OWMeshComponent::add(const std::vector<glm::vec3>& v)
{
	MeshData mdl;
	mdl.v3 = v;
	add(mdl);
}

void OWMeshComponent::add(const std::vector<glm::vec4>& v)
{
	MeshData mdl;
	mdl.v4 = v;
	add(mdl);
}

void OWMeshComponent::validate(const MeshData& md)
{
	if (md.v4.empty() == md.v3.empty())
	{
		if (md.v4.empty())
			throw NMSLogicException("LightRenderer has no data");
		else
			throw NMSLogicException("LightRenderer has both v3 and v4 data");
	}
}
