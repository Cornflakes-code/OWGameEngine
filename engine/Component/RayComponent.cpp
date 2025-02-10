#include "RayComponent.h"

#include "../Geometry/OWRay.h"
#include "../Geometry/BoundingBox.h"
#include "../Helpers/Shader.h"
#include "../Renderers/VAOBuffer.h"


RayComponent::RayComponent(OWActor* _owner, RayComponentData* _data)
	: ShapeComponent(_owner, _data), mRay(new OWRay(_data->origin, _data->direction))
{
}


bool RayComponent::intersects(const AABB& other) const
{
    glm::vec3 normal;
    float distance;
    return intersects(other, normal, distance);
}

bool RayComponent::intersects(const AABB& box, glm::vec3& normal, float& distance) const
{
    return mRay->intersects(box, normal, distance);
}

void RayComponent::doInit()
{
    std::vector<glm::vec3> vertices = mRay->vertices();
    Shader* lineShader = new Shader(&data()->shaderData);
    MeshDataLight lineData;
    lineData.vertices(vertices, GL_TRIANGLES);
    lineData.polygonMode(GL_FILL);
    lineData.colour(data()->colour, data()->shaderData.colourName);
    lineShader->setStandardUniformNames(data()->shaderData.PVMName);
    AABB b = AABB(vertices.begin(), vertices.begin() + 12);
    data()->boundingBox = b;
    VAOBuffer* vao = new VAOBuffer(lineShader, VAOBuffer::DRAW_ARRAYS);
    vao->blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vao->add(&lineData);
    vao->prepare();
    addRenderer(vao);
    ShapeComponent::doInit();
}
