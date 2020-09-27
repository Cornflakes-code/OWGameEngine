#include "Pyramid.h"

#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/CommonUtils.h"
#include "../Helpers/ResourceFactory.h"

Pyramid::Pyramid()
{
	Shader* sh = new Shader();
	sh->loadShaders(ResourceFactory::boilerPlateVertexShader(),
		ResourceFactory::boilerPlateFragmentShader(),
		ResourceFactory::boilerPlateGeometryShader());
	shader(sh, "pvm");
}

void Pyramid::setUp()
{
	std::vector<glm::vec4> v4;
	v4.push_back({ 0.0, 0.0, 0.0, 1.0 });
	v4.push_back({ 1.0, 0.0, 1.0, 1.0 });
	v4.push_back({ 0.0, 1.0, 1.0, 1.0 });
	v4.push_back({-1.0, 0.0, 1.0, 1.0 });
	v4.push_back({ 0.0,-1.0, 1.0, 1.0 });

	vertices(v4, 0, GL_TRIANGLES);
	colour(OWUtils::colour(OWUtils::SolidColours::MAGENTA), "colour");

	std::vector<unsigned int> ind = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,1,
		1,2,3, // Base
		3,4,1 // Base
	};
	indices(ind, GL_TRIANGLES);

	mRenderCallback
		= std::bind(&Pyramid::renderCallback,
			this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3, std::placeholders::_4);
}

void Pyramid::setPosition(const glm::vec3& newValue)
{
	mPosition.x = newValue.x;
	mPosition.y = newValue.y;
	mPosition.z = newValue.x;
}

void Pyramid::move(const glm::vec3& newValue)
{ 
	mPosition.x += newValue.x;
	mPosition.y += newValue.y;
	mPosition.z += newValue.x;
}

void Pyramid::renderCallback(
	const glm::mat4& proj, const glm::mat4& view,
	const glm::mat4& model, Shader* shader)
{
	shader->use();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}