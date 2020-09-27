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

FullScreen::FullScreen(Shader* _shader, const std::string& pvm)
{
	shader(_shader, "");

	mRenderCallback
		= std::bind(&FullScreen::renderCallback,
			this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3, std::placeholders::_4);

	mResizeCallback
		= std::bind(&FullScreen::resizeCallback,
			this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3);

}

void FullScreen::setUp(const AABB& world)
{
	float x = world.size().x / 4.0f;
	float y = world.size().y / 4.0f;
	float z = 0.0f;

	std::vector<glm::vec3> vert;
	vert.push_back({ -x, y, z });
	vert.push_back({ x, y, z });
	vert.push_back({ x, -y, z });
	vert.push_back({ -x, -y, z });

	vertices(vert, 0, GL_POINTS);
}

void FullScreen::renderCallback(const glm::mat4& proj, const glm::mat4& view,
	const glm::mat4& model, Shader* shader)
{
	OWUtils::PolygonModeRIAA poly;
	mShader->setVector2f("u_mouse", globals->pointingDevicePosition());
	mShader->setFloat("u_time", globals->secondsSinceLoad());

}

void FullScreen::resizeCallback(Shader* shader,
				ResizeHelper::ScaleByAspectRatioType scaleByAspectRatio,
				float aspectRatio)
{
	glm::vec2 vv = globals->physicalWindowSize();
	glm::vec2 v2 = scaleByAspectRatio({ vv });
	mShader->setVector2f("u_resolution", v2);
}
