#version 330 core

uniform mat4 mvp;

layout (location = 0) in vec3 in_position; // matches gl::EnableVertexAttribArray(0);
layout (location = 1) in vec2 in_uv; // matches gl::EnableVertexAttribArray(1);

out vec2 uv;

void main()
{
    gl_Position = mvp * vec4(in_position, 1.0f);
//    gl_Position = projMat * viewMat * modelMat * vec4(in_position, 1.0);
    uv = in_uv;
}
