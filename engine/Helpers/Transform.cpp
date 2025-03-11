#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

OWTransform::OWTransform(const OWTransform* _owner, const OWTransformData& _data)
	: mData(_data), mParent(_owner)
{
}

OWTransform::OWTransform(const OWTransform* _owner, const glm::vec3& pos,
	const glm::vec3& scale, const glm::quat& rot)
	: OWTransform(_owner, OWTransformData({ rot, pos, scale }))
{
}

void OWTransform::rotation(float radians, const glm::vec3& axis)
{
	mData.rotation = glm::angleAxis(radians, axis);
}

const glm::vec3 OWTransform::worldPosition() const
{
	const OWTransform* grandPa = mParent != nullptr ? mParent : nullptr;
	if (grandPa == nullptr)
		return mParent ? mParent->localPosition() + mData.position : mData.position;
	else
		return grandPa->localPosition() + mParent->localPosition() + mData.position;
}

const glm::mat4 OWTransform::modelMatrix() const
{
	glm::mat4 local = glm::mat4(rotation()) * glm::scale(glm::mat4(1), scale());
	const OWTransform* grandPa = mParent != nullptr ? mParent : nullptr;
	if (grandPa == nullptr)
	{
		glm::mat4 m = mParent ? mParent->modelMatrix() * local : local;
		return m;
	}
	else
	{
		glm::mat4 m = grandPa->modelMatrix() * mParent->modelMatrix() * local;
		return m;
	}
}

const glm::vec3 OWTransform::up() const
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
