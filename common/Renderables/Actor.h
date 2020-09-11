#pragma once

#include <glm/glm.hpp>

#include "Renderable.h"

/*
Scenery has the following properties:
	Lives in a scene. 
	Can move between scenes
	Can have props attached
	Can change a scene
*/
class Scene;

class Actor: public Renderable
{
public:
	const Scene* mScene;
	Actor(const Scene* _scene)
		: Renderable(_scene)
	{

	}
};