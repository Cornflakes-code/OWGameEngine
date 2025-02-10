#include "PlaneComponent.h"

#include "../Geometry/GeometricShapes.h"
#include "../Core/ErrorHandling.h"
#include "../Helpers/Shader.h"
#include "../Renderers/VAOBuffer.h"
#include "../Core/CommonUtils.h"

bool PlaneComponent::intersects(const AABB& box) const
{
    return false;
}

void PlaneComponent::doInit()
{
    std::vector<glm::vec3> vertices = GeometricShapes::rectangle(glm::vec2(1));
    Shader* lineShader = new Shader(&data()->shaderData);
    MeshDataLight lineData;
    lineData.vertices(vertices, GL_TRIANGLES);
    lineData.polygonMode(GL_FILL); 
    lineData.colour(data()->colour, data()->shaderData.colourName);
    // Now that we have copied the vertices into lineData, munge them a little bit so that the 
    // AABB is three dimensional;
    vertices[0].z -= 0.01f;
    vertices[1].z += 0.01f;
    AABB b = AABB(vertices);
    data()->boundingBox = b;
    VAOBuffer* vao = new VAOBuffer(lineShader, VAOBuffer::DRAW_ARRAYS);
    vao->blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vao->add(&lineData);
    vao->prepare();
    addRenderer(vao);
    ShapeComponent::doInit();
}

