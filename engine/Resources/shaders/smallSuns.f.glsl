#version 330 core

in vec4 coord;
uniform vec4 textcolor;
out vec4 fragOutColour;

void main() {
    float R = 1.0;
    float R2 = 0.5;
    float dist = sqrt(dot(coord.xy, coord.xy));
    if (dist >= R || dist <= R2) {
        discard;
    }
    float sm = smoothstep(R,R-0.01,dist);
    float sm2 = smoothstep(R2,R2+0.01,dist);
    float alpha = sm*sm2;
    fragOutColour = textcolor;
    //fragOutColour = vec4(0.0, 0.0, 1.0, alpha);
}
