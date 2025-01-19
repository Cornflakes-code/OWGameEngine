#include "Box.h"
#include <glm/glm.hpp>


#include "GeometricShapes.h"
#include "../Helpers/Shader.h"
#include "../Helpers/ShaderFactory.h"
#include "../Helpers/ComputeNormals.h"
#include "../Core/CommonUtils.h"
#include "../Renderers/VAOBuffer.h"

Box::Box(OWActor* _owner, const glm::vec3& _position)
	: OWSceneComponent(_owner, _position)
{}

bool Box::intersects(const AABB& box) const
{
	return false;
}

void Box::prepare(const glm::vec4& colour)
{
	std::vector<glm::vec3> vertices = GeometricShapes::cube();
	AABB b = AABB(vertices);
	bounds(b);
	ComputeNormals cn(vertices);
	MeshDataLight lineData = cn.compute();
	
	lineData.polygonMode(GL_FILL);
	//lineData.polygonMode(GL_LINE);

	Shader* wireShader = new Shader();
	wireShader->loadShaders("Wires.v.glsl",
		"Wires.f.glsl",
		ShaderFactory::boilerPlateGeometryShader());
	wireShader->setStandardUniformNames("", "projection", "view", "model");

	VAOBuffer* vao = new VAOBuffer(wireShader, VAOBuffer::DRAW_ARRAYS);
	vao->add(&lineData);
	auto pointRender = [colour](
		const glm::mat4& OW_UNUSED(proj),
		const glm::mat4& OW_UNUSED(view),
		const glm::mat4& OW_UNUSED(model),
		const glm::vec3& cameraPos,
		const Shader* shader)
		{
			shader->use();
			shader->setVector4f("lightColor", OWUtils::colour(OWUtils::SolidColours::WHITE));
			shader->setVector4f("objectColor", colour);// glm::vec4(0.90f, 0.91f, 0.98f, 1.0f)); // silver
			shader->setVector3f("viewLightPos", glm::vec3(160.0f, 60.0f, 50.0f));
			//shader->setVector3f("viewLightPos", cameraPos);
		};
	vao->prepare();
	wireShader->appendMutator(pointRender);
	addRenderer(vao);
	readyForRender(true);
}
