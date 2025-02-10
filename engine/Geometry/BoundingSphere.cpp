#include "BoundingSphere.h"

static float _sqr(float f)
{
	return f * f;
}

bool BoundingSphere::intersects(const OWBounding* other) const
{
	return false;
}

bool BoundingSphere::intersects(const AABB& box) const
{
	// https://stackoverflow.com/questions/28343716/sphere-intersection-test-of-aabb
	float dmin = 0;
	for (int i = 0; i < 3; i++) 
	{
		if (mPosition[i] < box.minPoint()[i])
		{
			dmin += _sqr(mPosition[i] - box.minPoint()[i]);
		}
		else if (mPosition[i] > box.maxPoint()[i])
		{
			dmin += _sqr(mPosition[i] - box.maxPoint()[i]);
		}
	}
	return dmin <= _sqr(mRadius);
}

/*
bool Ball::contains(const AABB<Dim, Type>& other) const
{
	// https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
	glm::vec4 ballCenter = center();

	// calculate AABB info (center, half-extents)
	glm::vec4 aabb_half_extents = other.size() / 2.0f;
	glm::vec4 aabb_center = other.center();

	// get difference vector between both centers
	glm::vec4 difference = ballCenter - aabb_center;
	glm::vec4 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	// add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec4 closest = aabb_center + clamped;

	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - ballCenter;

	return glm::length(difference) < mRadius;
}
*/

/*
* https://github.com/raysan5/raylib/blob/master/examples/models/models_rlgl_solar_system.c
// Draw sphere without any matrix transformation
// NOTE: Sphere is drawn in world position ( 0, 0, 0 ) with radius 1.0f
void DrawSphereBasic(Color color)
{
    int rings = 16;
    int slices = 16;

    // Make sure there is enough space in the internal render batch
    // buffer to store all required vertex, batch is reseted if required
    rlCheckRenderBatchLimit((rings + 2) * slices * 6);

    rlBegin(RL_TRIANGLES);
    rlColor4ub(color.r, color.g, color.b, color.a);

    for (int i = 0; i < (rings + 2); i++)
    {
        for (int j = 0; j < slices; j++)
        {
            glm::vec3(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * sinf(DEG2RAD * (j * 360 / slices)),
                sinf(DEG2RAD * (270 + (180 / (rings + 1)) * i)),
                cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * cosf(DEG2RAD * (j * 360 / slices)));
            glm::vec3(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
                sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
                cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
            glm::vec3(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * (j * 360 / slices)),
                sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
                cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * (j * 360 / slices)));

            glm::vec3(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * sinf(DEG2RAD * (j * 360 / slices)),
                sinf(DEG2RAD * (270 + (180 / (rings + 1)) * i)),
                cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * cosf(DEG2RAD * (j * 360 / slices)));
            glm::vec3(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
                sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))),
                cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
            glm::vec3(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
                sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
                cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
        }
    }
    rlEnd();
}
*/
