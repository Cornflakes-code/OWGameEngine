#include "Circle.h"


#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/CommonUtils.h"
#include "../Helpers/ResourceFactory.h"

Circle::Circle(const glm::vec3& initialPosition)
	:VertexSource(initialPosition)
{
	Shader* sh = new Shader();
	sh->loadShaders(ResourceFactory::boilerPlateVertexShader(),
		ResourceFactory::boilerPlateFragmentShader(),
		ResourceFactory::boilerPlateGeometryShader());
	shader(sh, "pvm");
}

void Circle::prepare()
{
	float radius = 20;
	float delta = glm::two_pi<float>() / 16;
	std::vector<glm::vec4> v4;
	for (float i = 0.0f; i < glm::two_pi<float>(); i += delta/3)
	{
		v4.push_back(glm::vec4(radius * glm::cos(i), radius * glm::sin(i),		
							   0.0, 1.0));
	}
	vertices(v4, 0, GL_LINE_LOOP);
	colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLACK), "colour");
}
