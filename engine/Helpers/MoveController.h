#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

#include "../Core/BoundingBox.h"

class OWENGINE_API MoveController
{
public:
	MoveController();
	void targetGeometry(const AABB& targetBounds, const glm::vec3& initialPosition);
	void move(const glm::vec4& speed);
	void direction(const glm::vec4& newValue) { mDirection = newValue; }
	void setPosition(const glm::vec3& newValue);
	void bounceIfCollide(const AABB& scenery);
	glm::mat4 translate(const glm::mat4& model) const;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	Compass::Direction wallIntersection(const AABB& scenery);
	glm::vec4 mDirection = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 mInitialPosition;
	AABB mBounds;
#pragma warning( pop )
};