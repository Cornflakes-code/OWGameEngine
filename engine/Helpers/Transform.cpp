#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Actor/OWActor.h"

OWTransform(OWActor* _owner, const OWTransformData& _data = OWTransformData())
	: mData(_data), mOwner(_owner) 
{
}

void OWTransform::rotation(float radians, const glm::vec3& axis)
{
	mData.rotation = glm::angleAxis(radians, axis);
}

const glm::vec3 OWTransform::worldPosition() const
{
	return mOwner ? mOwner->localPosition() + mData.position : mData.position;
}

const glm::mat4 OWTransform::modelMatrix() const
{
	glm::mat4 m = glm::mat4(rotation()) * glm::scale(glm::mat4(1), scale());
	m = glm::translate(m, mData.position);
	return mOwner ? mOwner->modelMatrix() * m : m;
}

const glm::vec3 OWTransform::up()
{
	const glm::vec4 v = modelMatrix() * glm::vec4(0, 1, 0, 0);
	return glm::normalize(glm::vec3(v.x, v.y, v.z));
}

const glm::vec3 OWTransform::inward() const
{
	const glm::vec4 v = modelMatrix() * glm::vec4(0, 0, 1, 0);
	return glm::normalize(glm::vec3(v.x, v.y, v.z));
}

const glm::vec3 OWTransform::right() const
{
	const glm::vec4 v = modelMatrix() * glm::vec4(1, 0, 0, 0);
	return glm::normalize(glm::vec3(v.x, v.y, v.z));
}
