#pragma once

#include <memory>

#include "../OWEngine/OWEngine.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Renderers/RenderTypes.h"

struct OWTransformData
{
	glm::quat rotation = {};
	glm::vec3 position = { 0, 0, 0 };
	glm::vec3 scale = { 1,1,1 };
	OWTransformData() {}
	OWTransformData(const glm::vec3& _position, const glm::vec3& _scale = { 1,1,1 })
		: position(_position), scale(_scale)
	{
	}
	OWTransformData(const glm::vec3& _position, const glm::vec3& _scale, const glm::quat& _rotation)
		: position(_position), scale(_scale), rotation(_rotation)
	{
	}
};

class OWENGINE_API OWTransform final
{
	OWTransformData mData;

	// The actor containing the entity that this transform is linked to
	const OWTransform* mParent = nullptr;
public:
	OWTransform(const OWTransformData& _data = OWTransformData());
	~OWTransform() {}

	const OWTransform* parentTransform() const { return mParent;  }
	void parentTransform(const OWTransform* newValue);
	void rotation(float radians, const glm::vec3& axis);
	glm::quat rotation() const {
		return mData.rotation;
	}
	void scale(const glm::vec3& newValue){
		mData.scale = newValue;
	}
	glm::vec3 scale() const {
		return mData.scale;
	}
	void localPosition(const glm::vec3& newValue) {
		mData.position = newValue;
	}
	glm::vec3 localPosition() const { 
		return mData.position;
	}
	const glm::vec3 worldPosition() const;
	const glm::mat4 modelMatrix() const;
	const glm::vec3 up() const;
	// inward is also known as forward
	const glm::vec3 inward() const;
	const glm::vec3 right() const;
	glm::vec2 drawSize(OWRenderTypes::DrawType _drawType) const;
};