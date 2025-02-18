#include "BoxComponent.h"
#include <glm/glm.hpp>


#include "../Geometry/GeometricShapes.h"
#include "../Helpers/Shader.h"
#include "../Helpers/ShaderFactory.h"
#include "../Helpers/ComputeNormals.h"
#include "../Core/CommonUtils.h"
#include "../Renderers/VAOBuffer.h"

BoxComponent::BoxComponent(OWActor* _owner, BoxComponentData* _data)
	: ShapeComponent(_owner, _data)
{}

bool BoxComponent::intersects(const AABB& other) const
{
	return false;
}

void BoxComponent::doInit()
{
	std::vector<glm::vec3> vertices = GeometricShapes::cube();
	data()->boundingBox = AABB(vertices);
	ComputeNormals cn(vertices);
	MeshDataLight lineData = cn.compute();
	
	lineData.polygonMode(data()->polygonMode);
	//lineData.polygonMode(GL_LINE);

	Shader* wireShader = new Shader(&data()->shaderData);

	VAOBuffer* vao = new VAOBuffer(wireShader, VAOBuffer::DRAW_ARRAYS);
	vao->add(&lineData);
	glm::vec4 colour = data()->colour;
	vao->prepare();
	addRenderer(vao);
	ShapeComponent::doInit();
}
