#include "ParticleComponent.h"

#include <glm/gtc/epsilon.hpp>

#include "../Geometry/OWSphere.h"
#include "../Geometry/BoundingBox.h"
#include "../Helpers/Shader.h"
#include "../Renderers/VAOBuffer.h"


ParticleComponent::ParticleComponent(OWActor* _owner, ParticleComponentData* _data)
    : ShapeComponent(_owner, _data)
{
}


bool ParticleComponent::intersects(const AABB& other) const
{
    return false;
}

void ParticleComponent::init()
{
	throw NMSException(std::stringstream()
		<< "Unimplemented function [ParticleComponent::init()].");
}
