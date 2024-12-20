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
	glm::vec3 mOrigen;
	glm::vec3 mDirection;
	glm::vec3 mInvDir;
	int mSign[3];
#pragma warning( pop )
public:
	Ray(OWActor* _owner, const glm::vec3& _position, const glm::vec3& orig, const glm::vec3& dir);
	bool intersects(const AABB& box) const;
};
