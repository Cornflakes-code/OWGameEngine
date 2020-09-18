#include "Axis.h"

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
#include "Points.h"
#include "Circle.h"

Axis::Axis(Shader* _shader)
	:Renderer(_shader)
{}

void Axis::setUp(const AABB& world, const Camera* camera)
{
	mPoints = new Points(new Shader("thebookofshaders.v.glsl", 
									"thebookofshaders.f.glsl", 
									"thebookofshaders.g.glsl"));

	float scale = 1.0;
	mLines = {
		{ world.center().x, world.center().y, world.center().z },
		{ world.maxPoint().x * scale, 0.0, 0.0 },
		{ 0.0, world.maxPoint().y * scale, 0.0 },
		{ 0.0, 0.0, world.maxPoint().z * scale} };

	int fontHeight = 12;
	glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
			fontHeight, camera->aspectRatio());
	//nice.first /= 2.0f;
	//nice.second /= 2.0f;

	TextBillboard* textX = new TextBillboardFixed("arial.ttf", fontHeight);
	TextBillboard* textY = new TextBillboardFixed("arial.ttf", fontHeight);
	TextBillboard* textZ = new TextBillboardFixed("arial.ttf", fontHeight);
	Circle* circle = new Circle();
	circle->setUp();
	mCircle.setUp(circle);
	textX->createText("X", nice.x, nice.y);
	textY->createText("Y", nice.x, nice.y);
	textZ->createText("Z", nice.x, nice.y);
	mPoints->setUp({ mLines[0] });

	textX->color({ 1.0, 0.0, 0.0, 1.0f });
	textY->color({ 0.0, 1.0, 0.0, 1.0f });
	textZ->color({ 0.0, 0.0, 1.0, 1.0f });
	mX.setUp(textX);
	mY.setUp(textY);
	mZ.setUp(textZ);

	std::vector<unsigned int> indices = { 0,1, 0,2, 0,3 };
	prepareOpenGL(mLines, indices, glm::vec4(1.0,1.0,1.0,1.0));
}

void Axis::render(const glm::mat4& proj, const glm::mat4& view, 
				  const glm::mat4& model) const
{
	{
		OWUtils::PolygonModeRIAA poly;
		renderOpenGL(proj, view, model, GL_LINES, []() {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		});
	}
	// Position the text at the ends of the lines
	mX.render(proj, view, glm::translate(model, mLines[1]));
	mY.render(proj, view, glm::translate(model, mLines[2]));
	mZ.render(proj, view, glm::translate(model, mLines[3]));
	mPoints->render(proj, view, model);
//	mCircle.render(proj, view, model);
}
