#pragma once

#include <glm/glm.hpp>

#include "VertexSource.h"
#include "../Helpers/MoveTarget.h"

/*
	Simple class wrapping creation and Rendering of a Pyramid. 
	Used for initial testing. Will be removed.
*/
class OWENGINE_API Pyramid: public VertexSource, public MoveTarget
{
public:
	Pyramid();
	void setUp();
private:
	void renderCallback(
		const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, Shader* shader);
};