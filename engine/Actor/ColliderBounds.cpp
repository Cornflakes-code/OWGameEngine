#include "ColliderBounds.h"

#include <Helpers/Shader.h>
#include <Component/TextComponent.h>
#include <Renderers/MeshRenderer.h>
#include <Renderers/RenderTypes.h>
#include <Geometry/GeometricShapes.h>

OWColliderBounds::OWColliderBounds(Scene* _scene, const std::string& _name)
	: OWActorDiscrete(_scene, _name, nullptr)
{
}

void OWColliderBounds::initialise()
{
	glm::vec4 colour = OWUtils::colour(OWUtils::SolidColours::BLACK);

	// CollisionType == CollisionType::Ovoid,
	{
		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = colour;
		sse.mesh = (new OWMeshComponent(this, "AABB Actor"))
			->setData(MeshData()
				.addVertices(OWGeometricShapes::circle())
				.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_LINE));
		Shader* shader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
		shader->setStandardUniformNames("pv");
		sse.rend = new OWMeshRenderer(shader,
			{
				GPUBufferObject::BufferType::Model,
				GPUBufferObject::BufferType::Colour
			},
			GPUBufferObject::BufferStyle::SSBO);
		addComponents(sse);
	}

	// CollisionType == CollisionType::Box
	{
		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = colour;
		sse.mesh = (new OWMeshComponent(this, "AABB Actor"))
			->setData(MeshData()
				.addVertices(OWGeometricShapes::cube())
				.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_LINE));
		Shader* shader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
		shader->setStandardUniformNames("pv");
		sse.rend = new OWMeshRenderer(shader,
			{
				GPUBufferObject::BufferType::Model,
				GPUBufferObject::BufferType::Colour
			},
			GPUBufferObject::BufferStyle::SSBO);
		addComponents(sse);
	}

	// CollisionType == CollisionType::Plane
	{
		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = colour;
		sse.mesh = (new OWMeshComponent(this, "AABB Actor"))
			->setData(MeshData()
				.addVertices(OWGeometricShapes::cube())
				.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_LINE));
		Shader* shader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
		shader->setStandardUniformNames("pv");
		sse.rend = new OWMeshRenderer(shader,
			{
				GPUBufferObject::BufferType::Model,
				GPUBufferObject::BufferType::Colour
			},
			GPUBufferObject::BufferStyle::SSBO);
		addComponents(sse);
	}

	// CollisionType == CollisionType::Point
	{
		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = colour;
		sse.mesh = (new OWMeshComponent(this, "AABB Actor"))
			->setData(MeshData()
				.addVertices(OWGeometricShapes::circle()) // A point is a very small circle.
				.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_LINE));
		Shader* shader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
		shader->setStandardUniformNames("pv");
		sse.rend = new OWMeshRenderer(shader,
			{
				GPUBufferObject::BufferType::Model,
				GPUBufferObject::BufferType::Colour
			},
			GPUBufferObject::BufferStyle::SSBO);
		addComponents(sse);
	}

	// CollisionType == CollisionType::Ray
	{
		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = colour;
		sse.mesh = (new OWMeshComponent(this, "AABB Actor"))
			->setData(MeshData()
				.addVertices(OWGeometricShapes::cube())
				.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_LINE));
		Shader* shader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
		shader->setStandardUniformNames("pv");
		sse.rend = new OWMeshRenderer(shader,
			{
				GPUBufferObject::BufferType::Model,
				GPUBufferObject::BufferType::Colour
			},
			GPUBufferObject::BufferStyle::SSBO);
		addComponents(sse);
	}

	// CollisionType == CollisionType::Permeable
	{
		OWActorDiscrete::DiscreteEntity sse;
		sse.colour = colour;
		sse.mesh = (new OWMeshComponent(this, "AABB Actor"))
			->setData(MeshData()
				.addVertices(OWGeometricShapes::cube())
				.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_LINE));
		Shader* shader = new Shader("Lines.v.glsl", "Lines.f.glsl", "");
		shader->setStandardUniformNames("pv");
		sse.rend = new OWMeshRenderer(shader,
			{
				GPUBufferObject::BufferType::Model,
				GPUBufferObject::BufferType::Colour
			},
			GPUBufferObject::BufferStyle::SSBO);
		addComponents(sse);
	}

}

void OWColliderBounds::addCollider(OWCollider* coll, OWActor* a, OWSize componentId)
{
}
