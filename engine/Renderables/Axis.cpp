#include "Axis.h"

#include <vector>
#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Camera.h"
#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"
#include "../Helpers/CommonUtils.h"
#include "../Helpers/ShaderFactory.h"

#include "VertexSourceRenderer.h"

#include "TextBillboardFixed.h"
#include "TextBillboardDynamic.h"
#include "Circle.h"
#include "VertexSource.h"

Axis::Axis()
{}

void Axis::prepare(const AABB& world)
{
	float scale = 1.0;

	std::vector<glm::vec3> axisCoords = {
		{ world.center().x, world.center().y, world.center().z },
		{ world.maxPoint().x * scale, 0.0, 0.0 },
		{ 0.0, world.maxPoint().y * scale, 0.0 },
		{ 0.0, 0.0, world.maxPoint().z * scale} };
	{
		int fontHeight = 12;
		glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
										fontHeight, globals->aspectRatio());
		//nice.first /= 2.0f;
		//nice.second /= 2.0f;
		mTextZero = new TextBillboardFixed(axisCoords[0], "arial.ttf", fontHeight);
		mTextX = new TextBillboardFixed(axisCoords[1], "arial.ttf", fontHeight);
		mTextY = new TextBillboardFixed(axisCoords[2], "arial.ttf", fontHeight);
		mTextZ = new TextBillboardFixed(axisCoords[3], "arial.ttf", fontHeight);

		mTextZero->createText("(0,0)", nice.x, nice.y);
		mTextX->createText("X", nice.x, nice.y);
		mTextY->createText("Y", nice.x, nice.y);
		mTextZ->createText("Z", nice.x, nice.y);

		mTextZero->colour({ 0.3, 0.45, 0.7, 1.0f }, "textcolor");
		mTextX->colour({ 1.0, 0.0, 0.0, 1.0f }, "textcolor");
		mTextY->colour({ 0.0, 1.0, 0.0, 1.0f }, "textcolor");
		mTextZ->colour({ 0.0, 0.0, 1.0, 1.0f }, "textcolor");

		mTextZero->addRenderer(new VertexSourceRenderer());
		mTextX->addRenderer(new VertexSourceRenderer());
		mTextY->addRenderer(new VertexSourceRenderer());
		mTextZ->addRenderer(new VertexSourceRenderer());
	}
	{
		ShaderFactory shaders;
		Shader* lineShader = new Shader();
		lineShader->loadShaders(shaders.boilerPlateVertexShader(),
			shaders.boilerPlateFragmentShader(),
			shaders.boilerPlateGeometryShader());
		mLines.shader(lineShader, "pvm");
		mLines.vertices(axisCoords, 0, GL_LINES);
		mLines.indices({ 0,1, 0,2, 0,3 }, GL_LINES);
		mLines.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLACK),
					"colour");
		mLines.addRenderer(new VertexSourceRenderer());
	}
	{
		mCircle.prepare();
		mCircle.addRenderer(new VertexSourceRenderer());
	}
	{
		//Shader* pointShader = new Shader("thebookofshaders.v.glsl",
		//								"solarSuns.f.glsl",
		//								//"text.f.glsl",
		//								"thebookofshaders_circle.g.glsl");
		Shader* pointShader = new Shader("smallSuns.v.glsl",
										"smallSuns.f.glsl",
										"");
//										"smallSuns.g.glsl");
		mZeroPoint.colour({ 0.1, 0.9, 0.1, 1 }, "textcolor");
		mZeroPoint.shader(pointShader, "pvm");
		glm::vec4 v4 = glm::vec4(axisCoords[0], 1.0);
		std::vector<glm::vec4> vv4;
		vv4.push_back(v4);
		mZeroPoint.vertices(vv4, 0, GL_POINTS);
		mZeroPoint.addRenderer(new VertexSourceRenderer());
	}
}

void Axis::render(const glm::mat4& proj, const glm::mat4& view, 
				  const glm::mat4& model) const
{
	auto pointRender = [](const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, Shader* shader) {
		shader->setVector2f("u_mouse", globals->pointingDevicePosition());
		shader->setFloat("u_time", globals->secondsSinceLoad());
	};
	auto pointResizeRender = [](Shader* shader,
		OWUtils::ScaleByAspectRatioType scaler,
		float aspectRatio) {
		glm::vec2 vv = globals->physicalWindowSize();
		//vv.x /= 20.0f;
		//vv.y /= 20.0f;
		//glm::vec2 v2 = scaler({ vv });
		shader->setVector2f("u_resolution", vv);
	};
	mZeroPoint.render(proj, view, model, nullptr, pointRender, pointResizeRender);

	mTextZero->render(proj, view, model);
	mTextX->render(proj, view, model);
	mTextY->render(proj, view, model);
	mTextZ->render(proj, view, model);
	mCircle.render(proj, view, model);
	mLines.render(proj, view, model, nullptr, [](const glm::mat4& proj, const glm::mat4& view,
				const glm::mat4& model, Shader* shader) {
				OWUtils::PolygonModeRIAA poly;
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			});
}
