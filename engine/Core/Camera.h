#pragma once

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

#include "UserInput.h"

/*
	Abstract interface for a Camera. It is intended that this class provides 
	the interface for 3D POV as well as a traditional fixed view of a scene
*/
class OWENGINE_API Camera
{
public:
	enum CameraType
	{
		ORTHO, FREE, SPHERICAL
	};

virtual void update() = 0;
virtual void pushCurrent() = 0;
virtual void restoreCurrent() = 0;
// return true if handled
virtual bool processInput(UserInput::AnyInput input, float seconds) = 0;
virtual void lookAt(const glm::vec3& target) = 0;
virtual glm::vec3 lookAt() const = 0;
//virtual void move(UserInput::BaseUserCommand dir, float seconds) = 0;
virtual glm::mat4 view() const = 0;
virtual glm::mat4 projection() const = 0;

virtual glm::vec3 up() const = 0;
virtual glm::vec3 right() const = 0;
virtual glm::vec3 direction() const = 0;

virtual void position(const glm::vec3& pos) = 0;
virtual glm::vec3 position() const = 0;

virtual void FOV(float radians) = 0;

virtual void clipping(float near_clip_distance, float far_clip_distance) = 0;

virtual void moveScale(float newValue) = 0;
virtual float moveScale() const = 0;

virtual void mouseScale(float newValue) = 0;
virtual float mouseScale() const = 0;

virtual void pitch(float radians) = 0;

virtual void yaw(float radians) = 0;
virtual void roll(float radians) = 0;

virtual void aspectRatio(float newValue) = 0;
virtual float aspectRatio() const = 0;
};