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

FullScreen::FullScreen(Shader* _shader)
	:Renderer(_shader)
{
}

void FullScreen::setUp(const AABB& world)
{
	float x = world.size().x / 4.0f;
	float y = world.size().y / 4.0f;
	float z = 0.0f;

	std::vector<glm::vec3> vertices;
	vertices.push_back({ -x, y, z });
	vertices.push_back({ x, y, z });
	vertices.push_back({ x, -y, z });
	vertices.push_back({ -x, -y, z });
	
	prepareOpenGL(vertices, std::vector<unsigned int>(), glm::vec4(1.0, 0.5, 1.0, 1.0));
}

void FullScreen::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model) const
{
	const glm::mat4 unity(1.0);
	OWUtils::PolygonModeRIAA poly;
	renderOpenGL(unity, unity, unity, GL_POINTS, [this, model]() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (aspectRatioModified())
		{
			glm::vec2 vv = globals->physicalWindowSize();
			glm::vec2 v2 = scaleByAspectRatio({ vv });
			glm::mat4 scaledModel = glm::scale(model, glm::vec3(v2, 0.0f));
			mShader->setVector2f("u_resolution", v2);
		}
		mShader->setVector2f("u_mouse", globals->pointingDevicePosition());
		mShader->setFloat("u_time", globals->secondsSinceLoad());
	});
}
