#include "Triangle.h"

#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/CommonUtils.h"

Triangle::Triangle(Shader* _shader)
	:Renderer(_shader)
{
}

void Triangle::setUp()
{
	std::vector<glm::vec3> vertices;
	vertices.push_back({ 0.0, 0.0, 0.0 });
	vertices.push_back({ 1.0, 0.0, 1.0 });
	vertices.push_back({ 0.0, 1.0, 1.0 });
	vertices.push_back({-1.0, 0.0, 1.0 });
	vertices.push_back({ 0.0,-1.0, 1.0 });

	std::vector<unsigned int> indices = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,1,
		1,2,3, // Base
		3,4,1 // Base
	};
	prepareOpenGL(vertices, indices, glm::vec4(0.0, 0.0, 1.0, 1.0));
}

void Triangle::setPosition(const glm::vec3& newValue)
{
	mPosition.x = newValue.x;
	mPosition.y = newValue.y;
	mPosition.z = newValue.x;
}

void Triangle::move(const glm::vec3& newValue)
{ 
	mPosition.x += newValue.x;
	mPosition.y += newValue.y;
	mPosition.z += newValue.x;
}

void Triangle::render(const glm::mat4& proj, 
					  const glm::mat4& view,
					  const glm::mat4& model) const
{
	OWUtils::PolygonModeRIAA poly;
	renderOpenGL(proj, view, model, GL_TRIANGLES, []() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	});
}
