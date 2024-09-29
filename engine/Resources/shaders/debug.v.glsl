//https://stackoverflow.com/questions/44793883/convert-floating-point-numbers-to-decimal-digits-in-glsl/44797902#44797902
//---------------------------------------------------------------------------
// Vertex
//---------------------------------------------------------------------------
#version 420 core
//---------------------------------------------------------------------------
layout(location=0) in vec4 vertex;
out vec2 pos;   // screen position <-1,+1>
void main()
    {
    pos=vertex.xy;
    gl_Position=vertex;
    }
//---------------------------------------------------------------------------
