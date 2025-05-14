#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Core/GlobalSettings.h>
#include <Core/ErrorHandling.h>

OWTransform::OWTransform(const OWTransformData& _data)
	: mData(_data)
{
}

void OWTransform::parentTransform(const OWTransform* newValue) 
{
	mParent = newValue;
	if (mParent == this)
		throw NMSLogicException("OWTransform::parentTransform(). Recursion");
}

void OWTransform::rotation(float radians, const glm::vec3& axis)
{
	//mData.rotation = glm::angleAxis(radians, axis);
	mData.rotation = axis * radians;
}

const glm::vec3 OWTransform::worldPosition() const
{
	// Order of transform calcs
	// https://gamedev.stackexchange.com/questions/196745/keeping-two-triangles-with-different-scale-attached-during-rotation?noredirect=1&lq=1
	const OWTransform* grandPa = mParent == nullptr ? nullptr : mParent->parentTransform();
	if (grandPa == nullptr)
		return mParent ? mParent->localPosition() + localPosition()  : localPosition();
	else
		return grandPa->localPosition() + mParent->localPosition() + localPosition();
}

const glm::mat4 OWTransform::modelMatrix() const
{
	glm::quat rotx = glm::angleAxis(rotation().x, glm::vec3(1, 0, 0));
	glm::quat roty = glm::angleAxis(rotation().y, glm::vec3(0, 1, 0));
	glm::quat rotz = glm::angleAxis(rotation().z, glm::vec3(0, 0, 1));
	glm::quat rot = rotx * rotz * roty;
	glm::mat4 local = glm::translate(glm::mat4(1.0f), localPosition()) * glm::mat4(glm::normalize(rot)) * glm::scale(glm::mat4(1.0f), scale());
	const OWTransform* grandPa = mParent == nullptr ? nullptr : mParent->parentTransform();
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

glm::vec2 OWTransform::drawSize(OWRenderTypes::DrawType _drawType) const
{
	if (_drawType == OWRenderTypes::DrawType::TwoDStatic)
	{
		glm::vec2 bbSize = { glm::length(glm::vec3(modelMatrix()[0])),
						glm::length(glm::vec3(modelMatrix()[1])) };
		// This seems to work best (trial and error) when resizing the window.
		float _aspectRatio = globals->physicalWindowSize().x /
			(globals->physicalWindowSize().y * 1.0f);
		if (_aspectRatio < 1)
		{
			bbSize.x /= _aspectRatio;
			bbSize.y *= _aspectRatio;
		}
		else
		{
			bbSize.x /= _aspectRatio;
			//retval.y *= _aspectRatio ;
		}
		return bbSize;
	}
	else if (_drawType == OWRenderTypes::DrawType::TwoDDynamic)
	{
		const glm::vec2& magic = { 5.4f, 3.6f };
		glm::vec2 bbSize({ magic.x * glm::length(glm::vec3(modelMatrix()[0])),
			magic.y * glm::length(glm::vec3(modelMatrix()[1])) });
		return bbSize;
	}
	else if (_drawType == OWRenderTypes::DrawType::ThreeD)
	{
		throw NMSNotYetImplementedException("OWTransform::drawSize not implemented for OWRenderTypes::DrawType::ThreeD. Cannot recover.");
	}
	else
	{
		throw NMSLogicException("OWTransform::drawSize(), Unknown DrawType[" + std::to_string(static_cast<int>(_drawType))
			+ "]. Cannot recover.");
	}
}
