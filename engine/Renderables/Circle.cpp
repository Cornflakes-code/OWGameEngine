#include "Circle.h"


#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/CommonUtils.h"
#include "../Helpers/ResourceFactory.h"

Circle::Circle()
	: mShader(new Shader())
{
	mShader->loadShaders(ResourceFactory::boilerPlateVertexShader(),
		ResourceFactory::boilerPlateFragmentShader(),
		ResourceFactory::boilerPlateGeometryShader());
}

void Circle::setUp()
{
	float radius = 20;
	float delta = glm::two_pi<float>() / 16;
	for (float i = 0.0f; i < glm::two_pi<float>(); i += delta)
	{
		mVertices.push_back(glm::vec4(radius * glm::cos(i), 
									  radius * glm::sin(i),		
									  0.0, 1.0));
	}
}

void Circle::setPosition(const glm::vec3& newValue)
{
	mPosition.x = newValue.x;
	mPosition.y = newValue.y;
	mPosition.z = newValue.x;
}

void Circle::move(const glm::vec3& newValue)
{
	mPosition.x += newValue.x;
	mPosition.y += newValue.y;
	mPosition.z += newValue.x;
}

void Circle::doRender(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model) const
{
	//OWUtils::PolygonModeRIAA poly;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mShader->use();
	glm::mat4 pvm = proj * view * model;
	mShader->setMatrix4("pvm", pvm);
}
