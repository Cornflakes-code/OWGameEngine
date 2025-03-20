#include "ThreeDAxis.h"

#include <Cameras/Camera.h>
#include <Core/GlobalSettings.h>
#include <Helpers/FreeTypeFontAtlas.h>
#include <Component/TextComponent.h>
#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>
#include <Core/Scene.h>
#include <Renderers/MeshRenderer.h>

ThreeDAxis::ThreeDAxis(Scene* _scene, const std::string& _name)
	: OWActorDiscrete(_scene, _name, nullptr)
{
}

void ThreeDAxis::initialise(const OWThreeDAxisData& _data)
{
	mData = _data;
	const AABB& bb = mData.bounds;
	std::vector<glm::vec3> axisCoords = {
		{ bb.center().x, bb.center().y, bb.center().z },
		{ bb.maxPoint().x, 0.0, 0.0 },
		{ 0.0, bb.maxPoint().y, 0.0 },
		{ 0.0, 0.0, bb.maxPoint().z }
	};

	transform(new OWTransform(nullptr)); // Always do this before creating child transforms
	OWActorDiscrete::DiscreteEntity sse;
	sse.colour = mData.axisColour;
	sse.coll = new OWCollider(this, OWCollider::CollisionType::Permeable);
	sse.mesh = (new OWMeshComponent(this, "XYZ Axis"))
		->add(MeshData()
		.addVertices(axisCoords)
		.addIndices({ 0,1, 0,2, 0,3 })
		.setModes(GL_LINES, GL_LINES, GL_FILL)
		.setColour(mData.axisColour, mData.axisColourName));

	Shader* shader = new Shader("");
	shader->setStandardUniformNames("pvm");
	sse.rend = new OWMeshRenderer(shader, 
			{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour },
		GPUBufferObject::BufferStyle::SSBO);
	sse.rend->drawModes(GL_LINES, GL_LINES);
	sse.trans = new OWTransform(transform());
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
}

OWActorDiscrete::DiscreteEntity ThreeDAxis::createText(const glm::vec3& pos, const std::string& s, unsigned int refPos, AABB& b)
{
	int fontHeight = 12;
	glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
		fontHeight, globals->camera()->aspectRatio());

	OWTextComponentData td;
	td.fontSpacing = nice;
	td.fontHeight = 12;
	td.colour = mData.labelColour;
	td.referencePos = refPos;
	td.text = s;
	td.tdt = OWTextComponentData::TextDisplayType::Static;

	OWActorDiscrete::DiscreteEntity sse;
	sse.coll = new OWCollider(this, OWCollider::CollisionType::Permeable);
	sse.mesh = new OWTextComponent(this, td.text, td);
	sse.colour = td.colour;
	Shader* shader = new Shader("textStaticBillboard.v.glsl", "text.f.glsl", "");
	shader->setStandardUniformNames("VP");
	shader->appendMutator(OWTextComponent::shaderMutator(td.tdt));
	sse.rend = new OWMeshRenderer(shader, 
		{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour },
		GPUBufferObject::BufferStyle::SSBO);
	glm::vec3 p = glm::vec3(pos.x, pos.y, pos.z);
	sse.trans = new OWTransform(this->transform(), p, {0.5f, 0.5f, 1.0f});
	return sse;
}
