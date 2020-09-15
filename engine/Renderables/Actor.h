#pragma once

#include <glm/glm.hpp>

#include "Renderable.h"

/*
	It is intended that a Scene contains three different types of renderable objects.
	Scenery Objects that do not interact with anything

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