#include "Points.h"

#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/CommonUtils.h"
#include "../Core/Movie.h"
#include "../Core/GLApplication.h"

Points::Points(Shader* _shader)
	:Renderer(_shader)
{
}

void Points::setUp(const std::vector<glm::vec3>& vertices)
{
	prepareOpenGL(vertices, std::vector<unsigned int>(), glm::vec4(0.0, 0.0, 1.0, 1.0));
}

void Points::setPosition(const glm::vec3& newValue)
{
	mPosition.x = newValue.x;
	mPosition.y = newValue.y;
	mPosition.z = newValue.x;
}

void Points::move(const glm::vec3& newValue)
{
	mPosition.x += newValue.x;
	mPosition.y += newValue.y;
	mPosition.z += newValue.x;
}

void Points::render(const glm::mat4& proj, 
					const glm::mat4& view, 
					const glm::mat4& model) const
{
	OWUtils::PolygonModeRIAA poly;
	renderOpenGL(proj, view, model, GL_POINTS, [this]() {
		mShader->setVector2f("u_resolution", theApp->movie()->physicalWindowSize());
		mShader->setVector2f("u_mouse", theApp->pointingDevicePosition());
		mShader->setFloat("u_time", theApp->secondsSinceLoad());
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	});
}
