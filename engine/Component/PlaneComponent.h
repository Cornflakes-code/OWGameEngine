#pragma once

#include <glm/glm.hpp>
#include "../Geometry/BoundingPlane.h"
#include "../Component/ShapeComponent.h"

class AABB;

struct OWENGINE_API PlaneComponentData: public ShapeComponentData
{
};

class OWENGINE_API PlaneComponent: public ShapeComponent
{
	
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	PlaneComponent(OWActor* _owner, PlaneComponentData* _data)
		: ShapeComponent(_owner, _data) {}
	bool intersects(const AABB& box) const override;
	void doInit() override;

#pragma warning( pop )
};


