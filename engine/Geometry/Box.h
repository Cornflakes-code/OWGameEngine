#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"
#include "../Core/Renderable.h"
#include "../Component/OWSceneComponent.h"

struct OWENGINE_API BoxComponentData: public OWSceneComponentData
{
};

// A box has parrellel sides
class OWENGINE_API BoxComponent: public OWSceneComponent
{
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )
public:
	Box(OWActor* _owner, BoxComponentData* _data);
	bool intersects(const AABB& box) const;
};
