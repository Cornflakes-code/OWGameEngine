#include "FullScreen.h"

#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <glm/gtx/transform.hpp>

#include "../Helpers/Shader.h"
#include "../Helpers/CommonUtils.h"
#include "../Core/Movie.h"
#include "../Core/GLApplication.h"
#include "../Core/GlobalSettings.h"

FullScreen::FullScreen(const glm::vec3& initialPosition,
					   const Shader* _shader, const std::string& pvm)
	: SimpleModel(initialPosition)
{
	shader(_shader, pvm);

	mRenderCallback
		= std::bind(&FullScreen::renderCallback,
			this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3, std::placeholders::_4);

	mResizeCallback
		= std::bind(&FullScreen::resizeCallback,
			this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3);
}

void FullScreen::prepare(const AABB& world)
{
	// Depending on the geometry shader it may not matter what we send.
	std::vector<glm::vec3> vert;
	vert.push_back({ 0.0f, 0.0f, 0.0f });
	vertices(vert, 0, GL_POINTS);
}

void FullScreen::renderCallback(const glm::mat4& proj, const glm::mat4& view,
	const glm::mat4& model, const Shader* shader)
{
	mShader->setVector2f("u_mouse", globals->pointingDevicePosition());
	mShader->setFloat("u_time", globals->secondsSinceLoad());

}

void FullScreen::resizeCallback(const Shader* shader,
				OWUtils::ScaleByAspectRatioType scaleByAspectRatio,
				float aspectRatio)
{
	glm::vec2 vv = globals->physicalWindowSize();
	mShader->setVector2f("u_resolution", vv);
}
