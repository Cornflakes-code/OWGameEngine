#pragma once

#include <glm/glm.hpp>
#include "BoundingBox.h"
#include "BoundingPlane.h"
#include "../Core/Renderable.h"
#include "../Core/OWSceneComponent.h"

// Note : This class defines infinite planes.
class OWENGINE_API Plane: public OWSceneComponent
{
	BoundingPlane mPlane;
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	Plane(OWActor* _owner, const glm::vec3& _position)
		: OWSceneComponent(_owner, _position) {}
	Plane(OWActor* _owner, const glm::vec3& _position, const glm::vec3& norm, const glm::vec3& p1)
		: OWSceneComponent(_owner, _position), mPlane(norm, glm::dot(norm, p1)){}

	Plane(OWActor* _owner, const glm::vec3& _position, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);
	Plane(OWActor* _owner, const glm::vec3& _position, const std::vector<glm::vec3>& corners);
	const glm::vec3& normal() const { return mPlane.normal(); }
	float getSignedDistanceToPlane(const glm::vec3& point) const
	{
		return mPlane.getSignedDistanceToPlane(point);
	}

	bool isOnOrForwardPlane(const AABB& box) const
	{
		return mPlane.isOnOrForwardPlane(box);
	}

#pragma warning( pop )
};


