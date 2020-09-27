#include "TextBillboard.h"

#include <iostream>
#include <algorithm>
#include <limits>
#include <experimental/filesystem>
#include <functional>

//#define GLM_FORCE_RADIANS

#include <glm/gtx/rotate_vector.hpp>

#include "ResourceFactory.h"
#include "ErrorHandling.h"
#include "Shader.h"

TextBillboard::TextBillboard(const std::string& fontFileName, int fontHeight)
{
	mFontData = globals->resourceCache()->loadFreeTypeFont(fontFileName, fontHeight);
}

TextBillboard::~TextBillboard()
{
//	globals->application()->removeWindowResizeListener(this);
}

void TextBillboard::createText(const std::string& text, float sx, float sy)
{
	mVec4 = mFontData->createText(text, sx, sy);
	texture(mFontData->texture());
	mBounds = findBounds();
	// move to origin ...
	mBounds = mBounds - mBounds.minPoint();
}

AABB TextBillboard::findBounds() const
{
	// Note: No depth value: default to 0;
	const float _max = std::numeric_limits<float>::max();
	glm::vec4 minPoint(_max, _max, 0, 1);
	glm::vec4 maxPoint(-_max, -_max, 0, 1);

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
	return AABB(minPoint, maxPoint);
}
