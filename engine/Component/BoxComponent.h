#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../OWEngine/OWEngine.h"
#include "../Component/ShapeComponent.h"

class AABB;
struct OWENGINE_API BoxComponentData: public ShapeComponentData
{
};

// A box has parrellel sides
class OWENGINE_API BoxComponent: public ShapeComponent
{
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( pop )
public:
	BoxComponent(OWActor* _owner, BoxComponentData* _data);
	bool intersects(const AABB& box) const override;
	void init() override;
};
