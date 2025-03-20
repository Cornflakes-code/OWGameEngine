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

const OWRenderData OWMeshComponent::renderData(AABB& bounds) const
{
	AABB b;
	for (const auto& m : mData)
	{
		b = b | m.bounds();
	}
	bounds = b;
	OWRenderData rd;
	rd.meshes = mData;
	return rd;
}

void OWMeshComponent::doSetup()
{
	for (const auto& m : mData)
	{
		validate(m);
	}
}
