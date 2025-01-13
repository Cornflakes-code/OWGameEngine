#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"

class OWENGINE_API OWShape
{
protected:
	std::vector<glm::vec3> mVertices;
public:
	virtual bool intersects(const OWShape* other) const { return false; }
	virtual std::vector<glm::vec3> vertices() = 0;
};

class OWENGINE_API OWCircle: public OWShape
{
	float mRadius;
	float mArcRadians;
public:
	OWCircle(float radius, float arcRadians)
		:mRadius(radius), mArcRadians(arcRadians) 
	{}
	virtual bool intersects(const OWShape* other) const { return other->intersects(this); }
	std::vector<glm::vec3> vertices() override;
};

class OWENGINE_API OWTorus: public OWShape
{
	float mInnerRadius;
	float mOuterRadius;
	float mArcRadians;
public:
	OWTorus(float innerRadius, float outerRadius, float arcRadians)
	:mInnerRadius(innerRadius),
		mOuterRadius(outerRadius),
		mArcRadians(arcRadians)
	{}
	std::vector<glm::vec3> vertices() override;
};

class OWENGINE_API OWRectangle : public OWShape
{
	glm::vec2 mDims;
public:
	OWRectangle(const glm::vec2& dims)
		: mDims(dims)
	{
	}
	std::vector<glm::vec3> vertices() override;
};

class OWENGINE_API OWGoldenRectangle: public OWShape
{
	float mScale;
public:
	OWGoldenRectangle(float scale)
		: mScale(scale)
	{
	}
	std::vector<glm::vec3> vertices() override;
};

class OWENGINE_API OWCube: public OWShape
{
	glm::vec3 mScale;
public:
	OWCube(const glm::vec3& scale = glm::vec3(1, 1, 1))
	: mScale(scale)
	{
	}
	std::vector<glm::vec3> vertices() override;
};

class OWENGINE_API OWStar: public OWShape
{
	float mInnerRadius;
	float mOuterRadius;
	unsigned int mNumPoints;
public:
	OWStar(float innerRadius, float outerRadius,
		unsigned int numPoints)
	: mInnerRadius(innerRadius),
	mOuterRadius(outerRadius),
	mNumPoints(numPoints)
	{}

	std::vector<glm::vec3> vertices() override;
};
