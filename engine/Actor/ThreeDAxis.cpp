#include "ThreeDAxis.h"

#include <Cameras/Camera.h>
#include <Core/GlobalSettings.h>
#include <Helpers/FreeTypeFontAtlas.h>
#include <Component/TextComponent.h>
#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>
#include <Core/Scene.h>
#include <Renderers/MeshRenderer.h>

ThreeDAxis::ThreeDAxis(Scene* _scene, const std::string& _name, const OWThreeDAxisData& _data)
	: OWActorSingle(_scene, _name, nullptr), mData(_data) 
{
}

void ThreeDAxis::doSetup()
{
	const AABB& bb = mData.bounds;
	std::vector<glm::vec3> axisCoords = {
	{ bb.center().x, bb.center().y, bb.center().z },
	{ bb.maxPoint().x, 0.0, 0.0 },
	{ 0.0, bb.maxPoint().y, 0.0 },
	{ 0.0, 0.0, bb.maxPoint().z }
};

	transform(new OWTransform(nullptr)); // Always do this before populating sse
	
	OWActorSingle::SingleSceneElement sse;
	sse.c = new OWCollider(this, OWCollider::CollisionType::Permeable);

	MeshData md;
	md.setColour(mData.axisColour, mData.axisColourName);
	md.setVertices(axisCoords, GL_LINES);
	md.setIndices({ 0,1, 0,2, 0,3 }, GL_LINES);
	sse.m = (new OWMeshComponent(this, "XYZ Axis"))->add(md);
	Shader* shader = new Shader("");
	shader->setStandardUniformNames("pvm");
	sse.r = new OWMeshRenderer(shader, OWMeshRenderer::DRAW_ARRAYS);
	sse.r->drawModes(GL_LINES, GL_LINES);
	sse.t = new OWTransform(transform());
	addComponents(sse);

	AABB boxUnion;
	AABB box;

	addComponents(createText(glm::vec3(0), "0", 
				OWTextComponentData::PositionType::Top | OWTextComponentData::PositionType::Right, box));
	//boxUnion |= box;
	addComponents(createText(axisCoords[1], "X", OWTextComponentData::PositionType::Center, box));
	//boxUnion |= box;
	addComponents(createText(axisCoords[2], "Y", OWTextComponentData::PositionType::Center, box));
	//boxUnion |= box;
	addComponents(createText(axisCoords[3], "Z", OWTextComponentData::PositionType::Center, box));
	//boxUnion |= box;
	//axis->renderBoundingBox(false);
	//boxUnion |= axis->constData()->boundingBox;

	OWActorSingle::doSetup();
}

OWActorSingle::SingleSceneElement ThreeDAxis::createText(const glm::vec3& pos, const std::string& s, unsigned int refPos, AABB& b)
{
	int fontHeight = 12;
	glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
		fontHeight, globals->camera()->aspectRatio());

	OWTextComponentData td;
	OWActorSingle::SingleSceneElement sse;
	sse.c = new OWCollider(this, OWCollider::CollisionType::Permeable);
	td.fontSpacing = nice;
	td.fontHeight = 12;
	td.colour = mData.labelColour;
	td.referencePos = refPos;
	td.text = s;
	td.tdt = OWTextComponentData::TextDisplayType::Static;
	sse.m = new OWTextComponent(this, td.text, td);
	sse.p = new OWPhysics();
	sse.r = new OWMeshRenderer("StaticText.json");
	sse.t = new OWTransform(nullptr, pos);
	return sse;
}
