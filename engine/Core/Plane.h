#pragma once

#include <glm/glm.hpp>
#include "BoundingBox.h"
#include "Renderable.h"

// Note : This class defines infinite planes.
class OWENGINE_API Plane :public Renderable
{
	glm::vec3 mNormal = { 0.f, 1.f, 0.f }; // unit vector
	float mDistance = 0.f; // distance from origin
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	Plane() = default;
	Plane(const glm::vec3& p1, const glm::vec3& norm)
		: mNormal(glm::normalize(norm)),
		mDistance(glm::dot(mNormal, p1))
	{}
	Plane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);
	Plane(const std::vector<glm::vec3>& corners);
	const glm::vec3& normal() const { return mNormal; }
	float getSignedDistanceToPlane(const glm::vec3& point) const
	{
		return glm::dot(mNormal, point) - mDistance;
	}

	bool isOnOrForwardPlane(const AABB& box) const;
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos) override;

#pragma warning( pop )
};


