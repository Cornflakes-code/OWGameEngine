#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"
#include "../Core/Renderable.h"
#include "../Core/OWSceneComponent.h"

class OWENGINE_API Ray: public OWSceneComponent
{
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec3 mDirection;
	glm::vec3 mInvDir;
	int mSign[3];
#pragma warning( pop )
public:
	Ray(OWActor* _owner, const glm::vec3& origin, const glm::vec3& dir);
	// returns true if ray goes through box else false Populates normal vector with 
	// distance is length of ray until intersection
	// distance is -ve if the AABB is behind this
	static bool rayIntersectsBox(const glm::vec3& origin, const glm::vec3& dir, const AABB& box, glm::vec3& normal, float& distance);
	bool intersects(const AABB& box, glm::vec3& normal, float& distance) const;
	// and old formulae
	bool intersects1(const AABB& box) const;
};
