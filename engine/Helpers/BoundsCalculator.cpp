#include "BoundsCalculator.h"

AABB BoundsCalculator::calcBounds(const std::vector<glm::vec3>& v) const
{
	{
		// Note: No depth value: default to 0;
		const float _max = std::numeric_limits<float>::max();
		glm::vec4 minPoint(_max, _max, 0, 1);
		glm::vec4 maxPoint(-_max, -_max, 0, 1);

		for (const auto& point : v)
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
		return AABB(minPoint, maxPoint);
	}
}
