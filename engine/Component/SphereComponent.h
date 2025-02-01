#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "../Component/ShapeComponent.h"

class OWSphere;
class AABB;

struct OWENGINE_API SphereComponentData: public ShapeComponentData
{
};

class OWENGINE_API SphereComponent : public ShapeComponent
{
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
#pragma warning( push )
#pragma warning( disable : 4251 )
	OWSphere* mSphere;
#pragma warning( pop )
public:
	SphereComponent(OWActor* _owner, SphereComponentData* _data);
	// returns true if ray goes through box else false Populates normal vector with 
	// distance is length of ray until intersection
	// distance is -ve if the AABB is behind this
	//bool intersects(const AABB& box, glm::vec3& normal, float& distance) const;
	bool intersects(const AABB& box) const override;
	void doInit() override;
};
