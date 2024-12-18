#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "BoundingBox.h"
#include "Renderable.h"

class OWENGINE_API Ray: public Renderable
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
	Ray(const glm::vec3& orig, const glm::vec3& dir);
	bool intersects(const AABB& box) const;
	void render(const glm::mat4& proj,
		const glm::mat4& view,
		const glm::mat4& model,
		const glm::vec3& cameraPos) override;
};
