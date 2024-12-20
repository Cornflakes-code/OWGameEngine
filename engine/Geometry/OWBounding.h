#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API OWBounding
{
public:
	virtual bool intersects(const OWBounding* other) const = 0;
	virtual void move(const glm::vec3& pt) = 0;
};
