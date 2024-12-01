#include "LightSource.h"

#include <Renderers/LightRenderer.h>
#include <Renderers/VAOBuffer.h>

#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>
#include <Helpers/GeometricShapes.h>
#include <Helpers/BoundsCalculator.h>
#define GLSL(src) "#version 330 core\n" #src

const std::string& lightSourceVertexShader()
{
    static std::string s =
        GLSL(layout(location = 0) in vec3 aPos;
    uniform mat4 pvm;
    void main()
    {
        gl_Position = pvm * vec4(aPos, 1.0);
    });
    return s;
}

LightSource::LightSource(const glm::vec3& _position)
    :mPosition(_position)
{
    mName = "Light";
/*
    Shader* shader = new Shader();
    shader->loadShaders(lightSourceVertexShader(),
        ShaderFactory::boilerPlateFragmentShader(),
        ShaderFactory::boilerPlateGeometryShader());
    shader->setStandardUniformNames("pvm");
*/
}

LightSource::~LightSource()
{

}

void LightSource::prepare()
{
    /*
    * An alternate way to draw spheres with triangles is at:
    * http://www.songho.ca/opengl/gl_sphere.html
    */
    Shader* sh = new Shader();
    sh->loadShaders(
        "sphere.v.glsl",
        "sphere.f.glsl",
        ShaderFactory::boilerPlateGeometryShader());
    sh->setStandardUniformNames("pvm", "perspective", "view", "model", "camPos");
    sh->setVector3f("sphereCenter", mPosition, true);
    MeshDataLight lineData;
    std::vector<glm::vec3> vertices = GeometricShapes::cube(mPosition);
    AABB b = BoundsCalculator().bounds(vertices);
    bounds(b);
    lineData.vertices(vertices, GL_TRIANGLES);

    VAOBuffer* vao = new VAOBuffer(sh, VAOBuffer::DRAW_ARRAYS);
    vao->add(&lineData);
    vao->prepare();
    addRenderer(vao);
    readyForRender();
}
