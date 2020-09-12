#include "Mazhar_Camera.h"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

// https://github.com/hmazhar/moderngl_camera
// Also see:
// http://www.songho.ca/opengl/gl_projectionmatrix.html
// https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/WebGL_model_view_projection
MazharCamera::MazharCamera()
{
}

MazharCamera::~MazharCamera() {
}

void MazharCamera::Reset()
{
}

void MazharCamera::Update()
{
	camera_direction = direction();

	if (camera_mode == ORTHO) 
	{
		//our projection matrix will be an orthogonal one in this case
		//if the values are not floating point, this UserCommand does not work properly
		//need to multiply by mAspect!!! (otherwise will not scale properly)
		projection = glm::ortho(float(-1.5 * mAspect), float(1.5 * mAspect), -1.5f, 1.5f, -10.0f, 10.f);
	} 
	else if (camera_mode == FREE) 
	{
		projection = glm::perspective(field_of_view, mAspect, near_clip, far_clip);
		//detmine axis for pitch rotation
		glm::vec3 axis = glm::cross(camera_direction, camera_up);
		//compute quaternion for pitch based on the camera pitch angle
		glm::quat pitch_quat = glm::angleAxis(camera_pitch, axis);
		//determine heading quaternion from the camera up vector and the heading angle
		glm::quat heading_quat = glm::angleAxis(camera_heading, camera_up);
		//add the two quaternions
		glm::quat temp = glm::cross(pitch_quat, heading_quat);
		temp = glm::normalize(temp);
		//update the direction from the quaternion
		camera_direction = glm::rotate(temp, camera_direction);
		//add the camera delta
		camera_position += camera_position_delta;
		//set the look at to be infront of the camera
		camera_look_at = camera_position + camera_direction * 1.0f;
		//damping for smooth camera
		camera_heading *= .5;
		camera_pitch *= .5;
		camera_position_delta = camera_position_delta * .8f;
	}
	//compute the MVP
	view = glm::lookAt(camera_position, camera_look_at, camera_up);
}

void MazharCamera::move(CameraDirection dir, float seconds)
{
	if (camera_mode == FREE) 
	{
		float speed = camera_scale * seconds;
		switch (dir) 
		{
			case UP:
				camera_position_delta += camera_up * speed;
				break;
			case DOWN:
				camera_position_delta -= camera_up * speed;
				break;
			case LEFT:
				camera_position_delta -= glm::cross(camera_direction, camera_up) * speed;
				break;
			case RIGHT:
				camera_position_delta += glm::cross(camera_direction, camera_up) * speed;
				break;
			case FORWARD:
				camera_position_delta += camera_direction * speed;
				break;
			case BACK:
				camera_position_delta -= camera_direction * speed;
				break;
		}
	}
}

void MazharCamera::changePitch(float radians)
{
	//Check bounds with the max pitch rate so that we aren't moving too fast
	if (radians < -max_pitch_rate)
	{
		radians = -max_pitch_rate;
	}
	else if (radians > max_pitch_rate)
	{
		radians = max_pitch_rate;
	}
	camera_pitch += radians;
	// Check bounds for the camera pitch
	if (camera_pitch > glm::two_pi<float>())
	{
		camera_pitch -= glm::two_pi<float>();
	}
	else if (camera_pitch < -glm::two_pi<float>())
	{
		camera_pitch += glm::two_pi<float>();
	}
	Update();
}

void MazharCamera::changeHeading(float radiansDelta)
{
	//Check bounds with the max heading rate so that we aren't moving too fast
	if (radiansDelta < -max_heading_rate) {
		radiansDelta = -max_heading_rate;
	} else if (radiansDelta > max_heading_rate) {
		radiansDelta = max_heading_rate;
	}
	// This controls how the heading is changed if the camera is pointed straight up or down
	// The heading delta direction changes
	if (camera_pitch > glm::half_pi<float>() && 
		 camera_pitch < glm::three_over_two_pi<float>() || 
	   (camera_pitch < -glm::half_pi<float>() && 
		 camera_pitch > -glm::three_over_two_pi<float>())) 
	{
		camera_heading -= radiansDelta;
	}
	else 
	{
		camera_heading += radiansDelta;
	}
	//Check bounds for the camera heading
	if (camera_heading > glm::two_pi<float>()) 
	{
		camera_heading -= glm::two_pi<float>();
	}
	else if (camera_heading < -glm::two_pi<float>())
	{
		camera_heading += glm::two_pi<float>();
	}
}

void MazharCamera::roll(float radiansDelta)
{
	camera_up = glm::rotate(camera_up, radiansDelta, camera_direction);

	// Just need to move the up vector
	//
	//glm::mat4 trans = glm::mat4(1.0f); 
	//glm::mat4 temp = glm::rotate(trans, radiansDelta, camera_direction);
	//glm::vec4 result = temp * 
	//camera_up = glm::rotate(trans, radiansDelta, camera_up);
}

void MazharCamera::Move2D(int x, int y)
{
	//compute the mouse delta from the previous mouse position
	glm::vec3 mouse_delta = mouse_position - glm::vec3(x, y, 0);
	// if the camera is moving, meaning that the mouse was clicked 
	// and dragged, change the pitch and heading
	if (move_camera) 
	{
		changeHeading(glm::radians<float>(0.08f * mouse_delta.x));
		changePitch(glm::radians<float>(0.08f * mouse_delta.y));
	}
	mouse_position = glm::vec3(x, y, 0);
}

//void MazharCamera::SetPos(int button, int state, int screenX, int screenY) {
//	if (button == 3 && state == GLUT_DOWN) {
//		camera_position_delta += camera_up * .05f;
//	} else if (button == 4 && state == GLUT_DOWN) {
//		camera_position_delta -= camera_up * .05f;
//	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//		move_camera = true;
//	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
//		move_camera = false;
//	}
//	mouse_position = glm::vec3(screenX, screenY, 0);
//}

void MazharCamera::GetMatricies(glm::mat4 &P, glm::mat4 &V) const
{
	P = projection;
	V = view;
}