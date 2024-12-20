#include "ThreeDAxis.h"

#include <Cameras/Camera.h>
#include <Core/OWActor.h>
#include <Core/GlobalSettings.h>
#include <Helpers/FreeTypeFontAtlas.h>
#include <Helpers/TextData.h>
#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>
#include <Renderers/VAOBuffer.h>
#include <Renderers/TextRendererStatic.h>
#include <Core/MeshComponent.h>

ThreeDAxis::ThreeDAxis(Scene* _owner, const glm::vec3& _position)
	: OWActor(_owner, _position)
{
	name("Labelled 3DAxis");
}

TextData* ThreeDAxis::createText(const glm::vec3& pos, const std::string& s, unsigned int refPos, AABB& b)
{
	int fontHeight = 12;
	glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
		fontHeight, globals->camera()->aspectRatio());

	float scale = 1.0;
	TextData* td = new TextData(this, pos, TextData::Static);
	td->font("arial.ttf", fontHeight);
	td->colour({ 1.0, 0.0, 0.0, 1.0f });
//	td->colour({ 0.3, 0.45, 0.7, 1.0f });
	td->spacing(nice.x, nice.y, glm::vec2(scale, scale), refPos);
	td->text(s);
	td->prepare();
	b = td->bounds();
	return td;
}

void ThreeDAxis::createAxisData(const AABB& w)
{
	const float scale = 1.0;
	std::vector<glm::vec3> axisCoords = {
		{ w.center().x, w.center().y, w.center().z },
		{ w.maxPoint().x * scale, 0.0, 0.0 },
		{ 0.0, w.maxPoint().y * scale, 0.0 },
		{ 0.0, 0.0, w.maxPoint().z * scale} };

	AABB boxUnion;
	AABB box;
	
	createText(glm::vec3(0), "0", TextData::Top | TextData::Right, box);
	boxUnion |= box;
	createText(axisCoords[1], "X", TextData::Center, box);
	boxUnion |= box;
	createText(axisCoords[2], "Y", TextData::Center, box);
	boxUnion |= box;
	createText(axisCoords[3], "Z", TextData::Center, box);
	boxUnion |= box;
	ShaderFactory shaders;
	Shader* lineShader = new Shader();
	lineShader->loadShaders(shaders.boilerPlateVertexShader(),
		shaders.boilerPlateFragmentShader(),
		shaders.boilerPlateGeometryShader());
	lineShader->setStandardUniformNames("pvm");
	MeshComponent* axis = new MeshComponent(this, glm::vec3(0));
	axis->name("Axis");
	MeshDataLight lineData;
	lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN), "colour");
	lineData.vertices(axisCoords, GL_LINES);
	lineData.indices({ 0,1, 0,2, 0,3 }, GL_LINES);
	axis->setup(&lineData, lineShader);
	boxUnion |= axis->bounds();
}

