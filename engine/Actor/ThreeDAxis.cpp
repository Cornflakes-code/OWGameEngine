#include "ThreeDAxis.h"

#include <Cameras/Camera.h>
#include <Actor/OWActor.h>
#include <Core/GlobalSettings.h>
#include <Helpers/FreeTypeFontAtlas.h>
#include <Component/TextComponent.h>
#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>
#include <Renderers/VAOBuffer.h>
#include <Renderers/TextRendererStatic.h>
#include <Component/MeshComponentLight.h>
#include <Core/Scene.h>

ThreeDAxis::ThreeDAxis(Scene* _scene, OWThreeDAxisScript* _script)
	: OWActor(_scene, _script)
{
	name("Labelled 3DAxis");
}

void ThreeDAxis::doInit()
{
	createAxisData(data()->axisData.world);
	OWActor::doInit();
}

TextComponent* ThreeDAxis::createText(const glm::vec3& pos, const std::string& s, unsigned int refPos, AABB& b)
{
	int fontHeight = 12;
	glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
		fontHeight, globals->camera()->aspectRatio());

	TextComponentData* td = new TextComponentData();
	td->physics.localMatrix = glm::translate(glm::mat4(1.0f), pos);
	td->textData.fontSpacing = nice;
	td->textData.fontHeight = 12;
	td->textData.colour = data()->axisData.labelColour;
	td->textData.fontScale = glm::vec2(1.0f);
	td->textData.referencePos = refPos;
	td->textData.text = s;
	td->textData.tdt = TextData::TextDisplayType::Static;
	TextComponent* tc = new TextComponent(this, td);
	tc->init();
	b = tc->constData()->boundingBox;
	return tc;
}

void ThreeDAxis::createAxisData(const AABB& w)
{
	AABB bb = data()->axisData.bounds;
	const float scale = 1.0;
	std::vector<glm::vec3> axisCoords = {
		{ w.center().x, w.center().y, w.center().z },
		{ w.maxPoint().x * scale, 0.0, 0.0 },
		{ 0.0, w.maxPoint().y * scale, 0.0 },
		{ 0.0, 0.0, w.maxPoint().z * scale} };

	AABB boxUnion;
	AABB box;
	
	createText(glm::vec3(0), "0", TextData::PositionType::Top | TextData::PositionType::Right, box);
	boxUnion |= box;
	createText(axisCoords[1], "X", TextData::PositionType::Center, box);
	boxUnion |= box;
	createText(axisCoords[2], "Y", TextData::PositionType::Center, box);
	boxUnion |= box;
	createText(axisCoords[3], "Z", TextData::PositionType::Center, box);
	boxUnion |= box;
	MeshComponentLightData* d = new MeshComponentLightData;
	d->meshData.colour(data()->axisData.axisColour, data()->axisData.axisColourName);
	d->meshData.vertices(axisCoords, GL_LINES);
	d->meshData.indices({ 0,1, 0,2, 0,3 }, GL_LINES);
	d->shaderData.PVMName = "pvm";
	d->name = "Axis";
	MeshComponentLight* axis = new MeshComponentLight(this, d);
	axis->init();
	//axis->renderBoundingBox(false);
	boxUnion |= axis->constData()->boundingBox;
}

