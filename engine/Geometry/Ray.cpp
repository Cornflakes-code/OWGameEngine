#include "Ray.h"

#include "GeometricShapes.h"
#include "../Helpers/Shader.h"
#include "../Renderers/VAOBuffer.h"

Ray::Ray(OWActor* _owner, const glm::vec3& origin, const glm::vec3& dir)
	: OWSceneComponent(_owner, origin), mRay(origin, dir)
{
}

bool Ray::intersects(const AABB& box, glm::vec3& normal, float& distance) const
{
	return mRay.intersects(box, normal, distance);
}

void Ray::prepare(const glm::vec4& colour)
{
    std::vector<glm::vec3> vertices = mRay.vertices();
    Shader* lineShader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
    MeshDataLight lineData;
    lineData.vertices(vertices, GL_TRIANGLES);
    lineData.polygonMode(GL_FILL);
    lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED), "uColour");
    //lineData.colour(colour, "uColour");
    lineShader->setStandardUniformNames("pvm");
    AABB b = AABB(vertices.begin(), vertices.begin() + 12);
    bounds(b);
    VAOBuffer* vao = new VAOBuffer(lineShader, VAOBuffer::DRAW_ARRAYS);
    vao->blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vao->add(&lineData);
    vao->prepare();
    addRenderer(vao);
    readyForRender(true);
}
