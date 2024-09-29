#include "LightSource.h"

#include <Renderers/LightRenderer.h>
#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>

float cubeSource[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

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

LightSource::LightSource()
{
    Shader* shader = new Shader();
    shader->loadShaders(lightSourceVertexShader(),
        ShaderFactory::boilerPlateFragmentShader(),
        ShaderFactory::boilerPlateGeometryShader());
    shader->setStandardUniformNames("pvm");
    mLightSource = new LightRenderer(shader);
}

LightSource::~LightSource()
{

}


