#include "Plane.h"

#include "BoundingBox.h"
#include "../Core/ErrorHandling.h"
#include "../Helpers/Shader.h"
#include "GeometricShapes.h"
#include "../Renderers/VAOBuffer.h"
#include "../Core/CommonUtils.h"

Plane::Plane(OWActor* _owner, const glm::vec3& _position, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4)
: OWSceneComponent(_owner, _position)
{
//	throw NMSException(std::string("Warning. The rest of this class defines infinite planes") +
//				"The intention of this ctor is to define a finite plane.");
}

Plane::Plane(OWActor* _owner, const glm::vec3& _position, const std::vector<glm::vec3>& corners)
	: Plane(_owner, _position, corners[0], corners[1], corners[2], corners[3] )
{
}

void Plane::prepare()
{
	std::vector<glm::vec3> vertices = GeometricShapes::rectangle(glm::vec2(1));
    AABB b = AABB(vertices);
    bounds(b);
    Shader* lineShader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
    MeshDataLight lineData;
    lineData.vertices(vertices, GL_TRIANGLES);
    lineData.polygonMode(GL_FILL);
    lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED), "uColour");
    lineShader->setStandardUniformNames("pvm");
    VAOBuffer* vao = new VAOBuffer(lineShader, VAOBuffer::DRAW_ARRAYS);
    vao->add(&lineData);
    vao->prepare();
    addRenderer(vao);
    readyForRender();
}

