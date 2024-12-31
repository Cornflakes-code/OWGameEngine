#pragma once

#include <glm/glm.hpp>
#include "BoundingBox.h"
#include "BoundingPlane.h"
#include "../Core/Renderable.h"
#include "../Core/OWSceneComponent.h"

class OWENGINE_API Plane: public OWSceneComponent
{
	
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	Plane(OWActor* _owner, const glm::vec3& _position)
		: OWSceneComponent(_owner, _position) {}
	void prepare(const glm::vec4& colour);
	//const glm::vec3& normal() const { return mPlane.normal(); }
	//float getSignedDistanceToPlane(const glm::vec3& point) const
	//{
	//	return mPlane.getSignedDistanceToPlane(point);
	//}

	//bool isOnOrForwardPlane(const AABB& box) const
	//{
	//	return mPlane.isOnOrForwardPlane(box);
	//}

#pragma warning( pop )
};


