#include "Vertices.h"
#include "VerticesRenderer.h"

#include "../Helpers/ErrorHandling.h"

Vertices::Vertices(const glm::vec3& _initialPosition)
: RenderableBase(_initialPosition)
{}

void Vertices::vertices(const std::vector<glm::vec3>& v,
								unsigned int location,
								unsigned int drawMode)
{
	mVec3 = v;
	mVertexLoc = location;
	mVertexMode = drawMode;
	assert(!mVec4.size() && mVec3.size());
	invalidateBounds();
}

void Vertices::vertices(const std::vector<glm::vec4>& v,
								unsigned int location,
								unsigned int drawMode)
{
	mVec4 = v;
	mVertexLoc = location;
	mVertexMode = drawMode;
	assert(!mVec3.size() && mVec4.size());
	invalidateBounds();
}

AABB Vertices::calcBounds() const
{
	// Note: No depth value: default to 0;
	const float _max = std::numeric_limits<float>::max();
	glm::vec4 minPoint(_max, _max, 0, 1);
	glm::vec4 maxPoint(-_max, -_max, 0, 1);

	if (mVec4.size())
	{
		for (const auto& point : mVec4)
		{
			if (point.x < minPoint.x)
				minPoint.x = point.x;
			else if (point.x > maxPoint.x)
				maxPoint.x = point.x;
			else if (point.y < minPoint.y)
				minPoint.y = point.y;
			else if (point.y > maxPoint.y)
				maxPoint.y = point.y;
		}
	}
	else
	{
		for (const auto& point : mVec3)
		{
			if (point.x < minPoint.x)
				minPoint.x = point.x;
			else if (point.x > maxPoint.x)
				maxPoint.x = point.x;
			else if (point.y < minPoint.y)
				minPoint.y = point.y;
			else if (point.y > maxPoint.y)
				maxPoint.y = point.y;
		}
	}
	return AABB(minPoint, maxPoint);
}
