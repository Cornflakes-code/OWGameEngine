#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

#include "SimpleModel.h"

/*
	Simple class wrapping creation and Rendering of a Triangle. Will be removed.
*/
class OWENGINE_API Circle : public SimpleModel
{
public:
	Circle(const glm::vec3& initialPosition);
	Circle()
		:Circle(glm::vec3(0.0f, 0.0f, 0.0f)) {}
	void prepare();
};