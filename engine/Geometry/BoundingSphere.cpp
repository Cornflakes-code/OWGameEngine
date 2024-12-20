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