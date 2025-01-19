#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>

#include "BoundingBox.h"
#include "Ray.h"
#include "BoundingSphere.h"
#include "../Core/CommonUtils.h"
#include "../Cameras/Camera.h"
#include "../Component/OWMovableComponent.h"


class OWENGINE_API PhysicalX: public OWMovableComponent
{
private:
public:
    PhysicalX(OWActor* _owner, const glm::vec3& _position) 
    : OWMovableComponent(_owner, _position) { }
};
