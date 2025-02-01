#include "SphereComponent.h"

#include "../Geometry/OWSphere.h"
#include "../Geometry/BoundingBox.h"
#include "../Helpers/Shader.h"
#include "../Renderers/VAOBuffer.h"


SphereComponent::SphereComponent(OWActor* _owner, SphereComponentData* _data)
	: ShapeComponent(_owner, _data)
{
}


bool SphereComponent::intersects(const AABB& other) const
{
    return false;
}

void SphereComponent::doInit()
{
    std::vector<glm::vec3> vertices = mSphere->vertices();
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
}
