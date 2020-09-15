#include "Triangle.h"

#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/CommonUtils.h"
#include "../Helpers/ResourceFactory.h"

Triangle::Triangle()
: mShader(new Shader())
{
	mShader->loadShaders(ResourceFactory::boilerPlateVertexShader(),
		ResourceFactory::boilerPlateFragmentShader(),
		ResourceFactory::boilerPlateGeometryShader());
}

void Triangle::setUp()
{
	mVertices.push_back({ 0.0, 0.0, 0.0, 1.0 });
	mVertices.push_back({ 1.0, 0.0, 1.0, 1.0 });
	mVertices.push_back({ 0.0, 1.0, 1.0, 1.0 });
	mVertices.push_back({-1.0, 0.0, 1.0, 1.0 });
	mVertices.push_back({ 0.0,-1.0, 1.0, 1.0 });

	mIndices = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,1,
		1,2,3, // Base
		3,4,1 // Base
	};
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

void Triangle::doRender(const glm::mat4& proj,
						const glm::mat4& view,
						const glm::mat4& model) const
{
	//OWUtils::PolygonModeRIAA poly;
	mShader->use();
	glm::mat4 pvm = proj * view * model;
	mShader->setMatrix4("pvm", pvm);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
