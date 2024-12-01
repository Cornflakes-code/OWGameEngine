#pragma once

#include <vector>
#include <string>

#include "MovableObject.h"
#include "ErrorHandling.h"
#include "BoundingBox.h"
#include "SceneGraphNode.h"
#include "../Renderers/RendererBase.h"

class OWENGINE_API Actor: public SceneGraphNode
{
protected: 
	Actor(Actor* _parent = nullptr): SceneGraphNode(_parent) {}
	virtual void attachObject(MovableObject*) {}
};
