#include "LightSource.h"

#include <Renderers/LightRenderer.h>
#include <Renderers/VAOBuffer.h>

#include <Helpers/ShaderFactory.h>
#include <Geometry/GeometricShapes.h>

#define GLSL(src) "#version 330 core\n" #src

// deferred rendering
// https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/deferredrendering/Tutorial%2015%20-%20Deferred%20Rendering.pdf
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


LightSource::LightSource(OWActor* _owner, LightSourceData* _data)
    : OWComponent(_owner), mData(_data)
{
        name(_data->name);
/*
    shader->loadBoilerPlates(),
    shader->setStandardUniformNames("pvm");
*/
}

LightSource::~LightSource()
{

}

void LightSource::doInit()
{
    /*
    * An alternate way to draw spheres with triangles is at:
    * http://www.songho.ca/opengl/gl_sphere.html
    */
    Shader* sh = new Shader(&mData->shaderData);
    MeshDataLight lineData;
    std::vector<glm::vec3> vertices = GeometricShapes::cube(mData->position);
    AABB b = AABB(vertices);
    lineData.vertices(vertices, GL_TRIANGLES);

    VAOBuffer* vao = new VAOBuffer(sh, VAOBuffer::DRAW_ARRAYS);
    vao->add(&lineData);
    vao->prepare();
    throw NMSException(std::stringstream()
        << "Unimplemented function [LightSource::doInit()].");
}
