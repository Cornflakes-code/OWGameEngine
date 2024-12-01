#include "BoundsCalculator.h"

AABB BoundsCalculator::calcBounds(const std::vector<glm::vec3>& v) const
{
	{
		// Note: No depth value: default to 0;
		constexpr float _max = std::numeric_limits<float>::max();
		glm::vec4 minPoint(_max, _max, 0, 1);
		glm::vec4 maxPoint(-_max, -_max, 0, 1);

		for (const auto& point : v)
		{
			if (point.x < minPoint.x)
				minPoint.x = point.x;
			if (point.x > maxPoint.x)
				maxPoint.x = point.x;

			if (point.y < minPoint.y)
				minPoint.y = point.y;
			if (point.y > maxPoint.y)
				maxPoint.y = point.y;

			if (point.z < minPoint.z)
				minPoint.z = point.z;
			if (point.z > maxPoint.z)
				maxPoint.z = point.z;
		}
		return AABB(minPoint, maxPoint);
	}
}
