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
	
	const AABB& bb = data()->axisData.axisSize;
	createAxisData(bb);
}

void ThreeDAxis::doInit()
{
	createAxisData(data()->axisData.world);
}

TextComponent* ThreeDAxis::createText(const glm::vec3& pos, const std::string& s, unsigned int refPos, AABB& b)
{
	int fontHeight = 12;
	glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
		fontHeight, globals->camera()->aspectRatio());

	TextComponentData* td = new TextComponentData();
	td->textData.fontSpacing = nice;
	td->textData.referencePos = refPos;
	td->textData.text = s;
	td->textData.tdt = TextData::TextDisplayType::Static;
	TextComponent* tc = new TextComponent(this, td);
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
	d->meshData.colour(data()->axisData.colour, "colour");
	d->meshData.vertices(axisCoords, GL_LINES);
	d->meshData.indices({ 0,1, 0,2, 0,3 }, GL_LINES);
	MeshComponentLight* axis = new MeshComponentLight(this, d);
	//axis->renderBoundingBox(false);
	std::string axisName = data()->axisData.axisName;
	axis->name(axisName);
	boxUnion |= axis->constData()->boundingBox;
}

