#pragma once

#include "BoundingBox.h"
class TextBillboard;
class Triangle;

class MovingText
{
	TextBillboard* mText;
	Triangle* mTriangle;
	AABB mBounds;
	glm::vec3 mVelocity;
	static OWUtils::Float mSpeed;
	glm::vec4 mDirection = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f); // (NorthWest)
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