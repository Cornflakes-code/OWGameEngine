#pragma once
#include <vector>

#include <glm/glm.hpp>

#include "../Helpers/Shader.h"
#include "OWSceneComponent.h"
#include <Core/CommonUtils.h>

struct OWENGINE_API ShapeComponentData: public OLDSceneComponentData
{
	ShaderData shaderData;
	glm::vec4 colour;
	unsigned int polygonMode = GL_LINE;
};

class OWENGINE_API ShapeComponent: public OLDSceneComponent
{
protected:
	ShapeComponentData* data()
	{
		return static_cast<ShapeComponentData*>(OLDSceneComponent::data());
	}
public:
	const ShapeComponentData* constData() const
	{
		return static_cast<const ShapeComponentData*>(OLDSceneComponent::constData());
	}

	ShapeComponent(OLDActor* _owner, ShapeComponentData* _data)
		: OLDSceneComponent(_owner, _data)
	{}
	virtual bool intersects(const AABB& box) const = 0;
protected:
private:
	friend class TextRenderer;
};
