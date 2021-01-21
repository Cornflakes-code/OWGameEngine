#pragma once

/*
	Simple class to calculate bounds of an array of points.
*/

#include "../Core/BoundingBox.h"


class BoundsCalculator
{
public:
	BoundsCalculator() {}
	void invalidate() const { mBoundsValid = false; }
	const AABB& bound(const std::vector<glm::vec3>& v) const
	{
		if (!mBoundsValid)
		{
			mBounds = calcBounds(v);
			mBoundsValid = true;
		}
		return mBounds;
	}

private:
	AABB calcBounds(const std::vector<glm::vec3>& v) const;
	mutable AABB mBounds;
	mutable bool mBoundsValid = false;
};