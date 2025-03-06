#pragma once

#include <memory>

#include "../OWEngine/OWEngine.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct OWTransformData
{
	glm::quat rotation = {};
	glm::vec3 position = { 0, 0, 0 };
	glm::vec3 scale = { 1,1,1 };
};

class OWENGINE_API OWTransform final
{
	OWTransformData mData;

	// The actor containing the entity that this transform is linked to
	OWTransform* mParent = nullptr;
public:
	OWTransform(OWTransform* _owner, const OWTransformData& _data);
	OWTransform(OWTransform* _owner, const glm::vec3& pos = glm::vec3(0),
		const glm::vec3& scale = glm::vec3(1), const glm::quat& rot = glm::quat());
	~OWTransform() {}

	const OWTransform* hostingTransform() const { return mParent;  }
	void hostingTransform(OWTransform* newValue) {
		mParent = newValue;
	}
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
};