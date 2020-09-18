#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
/*
	Saves all ScenePhysicsState and camera by them streaming to this.
	As well as writing to file also provides in-memory state for the MacroRecorder.
*/
class OWENGINE_API SaveAndRestore
{
public:
	SaveAndRestore();
	glm::uvec2 physicalWindowSize() const;
	std::string activeScene() const;
};

