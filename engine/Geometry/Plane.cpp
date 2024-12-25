#include "Plane.h"

#include "BoundingBox.h"
#include "../Core/ErrorHandling.h"
#include "../Helpers/Shader.h"
#include "GeometricShapes.h"
#include "../Renderers/VAOBuffer.h"
#include "../Core/CommonUtils.h"

void Plane::prepare()
{
	std::vector<glm::vec3> vertices = GeometricShapes::rectangle(glm::vec2(1));
    AABB b = AABB(vertices);
    bounds(b);
    Shader* lineShader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
    MeshDataLight lineData;
    lineData.vertices(vertices, GL_TRIANGLES);
    lineData.polygonMode(GL_FILL); 
    //lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED), "uColour");
    lineData.colour({ 1.0f, 0.33f, 0.33f, 0.2f }, "uColour");
    lineShader->setStandardUniformNames("pvm");
    VAOBuffer* vao = new VAOBuffer(lineShader, VAOBuffer::DRAW_ARRAYS);
    vao->blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vao->add(&lineData);
    vao->prepare();
    addRenderer(vao);
    readyForRender();
}

