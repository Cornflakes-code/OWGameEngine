#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

#include "../Renderables/VertexSource.h"

class Shader;
/*
	Simple class wrapping creation and Rendering of a Triangle. Will be removed.
*/
class OWENGINE_API Circle : public VertexSource
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec3 mPosition = glm::vec3(0);
#pragma warning( pop )
public:
	Circle();
	void setUp();
	void setPosition(const glm::vec3& newValue);
	void move(const glm::vec3& newValue);
};