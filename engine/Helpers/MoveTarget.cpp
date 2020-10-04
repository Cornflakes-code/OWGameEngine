#include "MoveTarget.h"

MoveTarget::MoveTarget()
{}


void MoveTarget::setPosition(const glm::vec3& newValue)
{
	mPosition.x = newValue.x;
	mPosition.y = newValue.y;
	mPosition.z = newValue.x;
}

void MoveTarget::move(const glm::vec3& newValue)
{
	mPosition.x += newValue.x;
	mPosition.y += newValue.y;
	mPosition.z += newValue.x;
}