#version 430 core

// https://blog.42yeah.is/rendering/opengl/2023/12/18/perfect-ellipsoid.html
in vec3 worldPos;
out vec4 color;

uniform vec3 camPos;
uniform vec3 sphereCenter;


vec3 sphereIntersect(vec3 c, vec3 ro, vec3 p) {
    vec3 rd = vec3(normalize(p - ro));
    vec3 u = vec3(ro - c); // ro relative to c

    float a = dot(rd, rd);
    float b = 2.0 * dot(u, rd);
    float cc = dot(u, u) - 1.0;

    float discriminant = b * b - 4 * a * cc;

    // no intersection
    if (discriminant < 0.0) {
        return vec3(0.0);
    }

    float t1 = (-b + sqrt(discriminant)) / (2.0 * a);
    float t2 = (-b - sqrt(discriminant)) / (2.0 * a);
    float t = min(t1, t2);
    vec3 intersection = ro + vec3(t * rd);

    return intersection;
}

void main() {
    vec3 sp = sphereIntersect(sphereCenter, camPos, worldPos);
    if (sp == vec3(0.0)) {
		discard;
    }
	vec3 fancy = sp - sphereCenter;
	color = vec4(abs(fancy), 1.0);
}
