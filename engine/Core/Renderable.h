#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API Renderable
{
	public:
		virtual void render(const glm::mat4& proj,
			const glm::mat4& view,
			const glm::mat4& model,
			const glm::vec3& cameraPos) = 0;
};
