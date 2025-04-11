#include "Button.h"

#include <glm/gtx/transform.hpp>

#include <Helpers/Shader.h>
#include <Component/TextComponent.h>
#include <Renderers/MeshRenderer.h>
#include <Renderers/RenderTypes.h>
#include <Geometry/GeometricShapes.h>

OWButton::OWButton(Scene* _scene, const std::string& _name)
	: OWActorDiscrete(_scene, _name, nullptr)
{
}

OWActorDiscrete::DiscreteEntity OWButton::makeShape(const std::string& s, const glm::vec4& colour, const glm::vec3& size)
{
	std::vector<glm::vec3> rectCoords = {
		{ -0.5f, 0.5f, 0.0f },
		{ 0.5f, 0.5f, 0.0f },
		{ 0.5f, -0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f }
	};
	OWActorDiscrete::DiscreteEntity sse;
	sse.colour = colour;
	sse.coll = new OWCollider(this, OWCollider::CollisionType::Permeable);
	sse.mesh = (new OWMeshComponent(this, s))
		->add(MeshData()
			//.addVertices(triangles)
			.addVertices(OWGeometricShapes::goldenRectangle())
			.addIndices({ 0, 1, 2, 2, 3, 0 })
			.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_FILL));
	sse.mesh->drawType(OWRenderTypes::DrawType::TwoDStatic);

	Shader* shader = new Shader("textStaticBillboard.v.glsl", "text_1.f.glsl", "");
	shader->setStandardUniformNames("pv");
	sse.rend = new OWMeshRenderer(shader,
		{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour,
				GPUBufferObject::BufferType::BillboardSize },
		GPUBufferObject::BufferStyle::SSBO);
	sse.rend->drawModes(GL_TRIANGLES, GL_TRIANGLES);
	OWTransform* trans = (new OWTransform({ glm::vec3(0), size }));
	trans->parentTransform(transform());
	sse.physics = new OWPhysics(trans);
	return sse;
}

void OWButton::initialise(const OWButtonData& _data)
{
	std::vector<glm::vec3> triangles = {
		{ -0.5f, 0.5f, 0.0f },
		{ 0.5f, 0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{ 0.5f, 0.5f, 0.0f },
		{ 0.5f, -0.5f, 0.0f }
	};
	this->transform(new OWTransform()); // Always do this before creating child transforms

	glm::vec3 sz = glm::vec3(0.4, 0.1, 0);
	addComponents(makeShape("Unclicked Button", OWUtils::colour(OWUtils::SolidColours::BLUE), sz));
	addComponents(makeShape("Clicked Button", OWUtils::colour(OWUtils::SolidColours::RED), sz * glm::vec3(0.7f, 0.7f, 1.0f)));

	OWTextComponentData td;
	td.tdt = OWRenderTypes::DrawType::TwoDStatic;;
	Shader* shader = new Shader("textStaticBillboard.v.glsl", "text.f.glsl", "");
	shader->setStandardUniformNames("pv");
	shader->appendMutator(OWTextComponent::shaderMutator(td.tdt));
	OWActorDiscrete::DiscreteEntity sse;
	sse.rend = (new OWMeshRenderer(shader,
		{ GPUBufferObject::BufferType::Position,
			GPUBufferObject::BufferType::Colour,
			GPUBufferObject::BufferType::BillboardSize },
		GPUBufferObject::BufferStyle::SSBO));
	td.text = "Click Me";
	sse.mesh = new OWTextComponent(this, "Rope Banner", td);
	OWTransform* trans = new OWTransform(OWTransformData(glm::vec3(0, 0, 0), { 0.05f, 0.05f, 1.0f }));
	trans->parentTransform(transform());
	sse.physics = new OWPhysics(trans);

	sse.physics = new OWPhysics(new OWTransform({ glm::vec3(0, 0, 0), { 0.05f, 0.05f, 1.0f } }));
	sse.colour = OWUtils::colour(OWUtils::SolidColours::GREEN);
	sse.coll = new OWCollider(this, OWCollider::CollisionType::Box);
	addComponents(sse);
}
