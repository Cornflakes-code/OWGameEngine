#pragma once

#include <glm/glm.hpp>

#include "../Renderables/Vertices.h"

class MoveTarget
{
public:
	MoveTarget();
	void addTarget(Vertices* source) { mSource = source; }
	void setPosition(const glm::vec3& newValue);
	void move(const glm::vec3& newValue);
	AABB bounds() const { return mSource->bounds(); }
private:
	Vertices* mSource = nullptr;
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec3 mPosition = glm::vec3(0);
#pragma warning( pop )
};