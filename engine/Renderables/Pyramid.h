#pragma once

#include <glm/glm.hpp>

#include "../Renderables/VertexSource.h"

class Shader;
/*
	Simple class wrapping creation and Rendering of a Pyramid. 
	Used for initial testing. Will be removed.
*/
class OWENGINE_API Pyramid: public VertexSource
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::vec3 mPosition = glm::vec3(0);
#pragma warning( pop )
public:
	Pyramid();
	void setUp();
	void setPosition(const glm::vec3& newValue);
	void move(const glm::vec3& newValue);
private:
	void renderCallback(
		const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, Shader* shader);
};