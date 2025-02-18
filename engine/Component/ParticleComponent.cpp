#include "ParticleComponent.h"

#include <glm/gtc/epsilon.hpp>

#include "../Geometry/OWSphere.h"
#include "../Geometry/BoundingBox.h"
#include "../Helpers/Shader.h"
#include "../Renderers/VAOBuffer.h"

/*
Sprites
Game Art 2D
Online texture packer tool code web
vertex shader outputs Pos in NDC (w=1)
Spritesheet texture coords
Screen Space to NDC
X(ndc) = 2 * X(screen) / window_width - 1
Y(ndc) = 2 * Y(screen) / window_height - 1
*/
ParticleComponent::ParticleComponent(OWActor* _owner, ParticleComponentData* _data)
    : ShapeComponent(_owner, _data)
{
}


bool ParticleComponent::intersects(const AABB& other) const
{
    return false;
}

void ParticleComponent::doInit()
{
	throw NMSException(std::stringstream()
		<< "Unimplemented function [ParticleComponent::doInit()].");
	ShapeComponent::doInit();
}
