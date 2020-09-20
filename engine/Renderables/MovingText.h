#pragma once

#include "../OWEngine/OWEngine.h"

#include "BoundingBox.h"
#include "SimpleVertexRender.h"


class Pyramid;
class TextBillboard;
/*
	A development class wrapping very simple collision functionality. Will be removed
	in later iterations
*/
class OWENGINE_API MovingText
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	Pyramid* mPyramid;
	SimpleVertexRender mText;
	SimpleVertexRender mPyramidRender;
	AABB mBounds;
	glm::vec3 mVelocity;
	glm::vec4 mDirection = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f); // (NorthWest)
	static OWUtils::Float mSpeed;
#pragma warning( pop )
public:
	MovingText();
	void direction(const glm::vec4& newValue) { mDirection = newValue; }
	void text(TextBillboard* newValue);
	void fixedTimeStep(float seconds);
	void velocity(const glm::vec3& newValue) { mVelocity = newValue; }
	Compass::Direction wallIntersection(const AABB& scenery);
	void bounceIfCollide(const AABB& scenery);
	void setPosition(const glm::vec3& newValue);
	void move(const glm::vec4& velocity);
	void render(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& model) const;
};