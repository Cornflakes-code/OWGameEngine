#include "Axis.h"

#include <vector>
#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Camera.h"
#include "../Helpers/Shader.h"
#include "../Helpers/CommonUtils.h"

#include "TextBillboardFixed.h"
#include "TextBillboardDynamic.h"
#include "Circle.h"
#include "VertexSource.h"

Axis::Axis()
{}

void Axis::setUp(const AABB& world, const Camera* camera)
{
	float scale = 1.0;

	mAxisCoords = {
		{ world.center().x, world.center().y, world.center().z },
		{ world.maxPoint().x * scale, 0.0, 0.0 },
		{ 0.0, world.maxPoint().y * scale, 0.0 },
		{ 0.0, 0.0, world.maxPoint().z * scale} };
	{
		int fontHeight = 12;
		glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
			fontHeight, camera->aspectRatio());
		//nice.first /= 2.0f;
		//nice.second /= 2.0f;
		TextBillboard* textX = new TextBillboardFixed("arial.ttf", fontHeight);
		TextBillboard* textY = new TextBillboardFixed("arial.ttf", fontHeight);
		TextBillboard* textZ = new TextBillboardFixed("arial.ttf", fontHeight);
		textX->createText("X", nice.x, nice.y);
		textY->createText("Y", nice.x, nice.y);
		textZ->createText("Z", nice.x, nice.y);

		textX->colour({ 1.0, 0.0, 0.0, 1.0f }, "textcolor");
		textY->colour({ 0.0, 1.0, 0.0, 1.0f }, "textcolor");
		textZ->colour({ 0.0, 0.0, 1.0, 1.0f }, "textcolor");

		mTextX.addResizer(new ResizeHelper());
		mTextX.addSource(textX);
		mTextY.addResizer(new ResizeHelper());
		mTextY.addSource(textY);
		mTextZ.addResizer(new ResizeHelper());
		mTextZ.addSource(textZ);
	}
	{
		Shader* lineShader = new Shader();
		lineShader->loadShaders(ResourceFactory::boilerPlateVertexShader(),
			ResourceFactory::boilerPlateFragmentShader(),
			ResourceFactory::boilerPlateGeometryShader());
		VertexSource* vsline = new VertexSource();
		vsline->shader(lineShader, "pvm");
		vsline->vertices(mAxisCoords, 0, GL_LINES);
		vsline->indices({ 0,1, 0,2, 0,3 }, GL_LINES);
		vsline->colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLACK),
			"colour");
		mLines.addSource(vsline);
	}
	{
		Circle* circle = new Circle();
		circle->setUp();
		mCircle.addSource(circle);
	}
	{
		Shader* pointShader = new Shader("thebookofshaders.v.glsl",
										"thebookofshaders.f.glsl",
										"thebookofshaders_circle.g.glsl");
		VertexSource* p = new VertexSource();
		p->shader(pointShader, "pvm");
		p->vertices({ mAxisCoords[0] }, 0, GL_POINTS);
		mPoints.addResizer(new ResizeHelper());
		mPoints.addSource(p);
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
		ResizeHelper::ScaleByAspectRatioType scaler,
		float aspectRatio) {
		glm::vec2 vv = globals->physicalWindowSize();
		glm::vec2 v2 = scaler({ vv });
		shader->setVector2f("u_resolution", v2);
	};
	mPoints.render(proj, view, model, pointRender, pointResizeRender);

	// Position the text at the ends of the lines
	mTextX.render(proj, view, glm::translate(model, mAxisCoords[1]));
	mTextY.render(proj, view, glm::translate(model, mAxisCoords[2]));
	mTextZ.render(proj, view, glm::translate(model, mAxisCoords[3]));
	mCircle.render(proj, view, model);
	mLines.render(proj, view, model, [](const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, Shader* shader) {
		OWUtils::PolygonModeRIAA poly;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	});
}
