#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"
#include "../Core/Renderable.h"
#include "../Core/OWSceneComponent.h"

// A box has parrellel sides
class OWENGINE_API Box : public OWSceneComponent
{
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )
public:
	Box(OWActor* _owner, const glm::vec3& _position);
	void prepare(const glm::vec4& colour);
	bool intersects(const AABB& box) const;
};
