#include "Plane.h"

#include "BoundingBox.h"
#include "../Core/ErrorHandling.h"
#include "../Helpers/Shader.h"
#include "GeometricShapes.h"
#include "../Renderers/VAOBuffer.h"
#include "../Core/CommonUtils.h"

void Plane::prepare(const glm::vec4& colour)
{
	std::vector<glm::vec3> vertices = GeometricShapes::rectangle(glm::vec2(1));
    Shader* lineShader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
    MeshDataLight lineData;
    lineData.vertices(vertices, GL_TRIANGLES);
    lineData.polygonMode(GL_FILL); 
    //lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED), "uColour");
    lineData.colour(colour, "uColour");
    lineShader->setStandardUniformNames("pvm");
    // Now that we have copied the vertices into lineData, munge them a little bit so that the 
    // AABB is three dimensional;
    vertices[0].z -= 0.01;
    vertices[1].z += 0.01;
    AABB b = AABB(vertices);
    bounds(b);
    VAOBuffer* vao = new VAOBuffer(lineShader, VAOBuffer::DRAW_ARRAYS);
    vao->blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vao->add(&lineData);
    vao->prepare();
    addRenderer(vao);
    readyForRender(true);
}

