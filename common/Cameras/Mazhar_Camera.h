/*
 camera.h
 OpenGL Camera Code
 Capable of 2 modes, orthogonal, and free
 Quaternion camera code adapted from: http://hamelot.co.uk/visualization/opengl-camera/
 Written by Hammad Mazhar
 */
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
/*
	Cameras
	google - opengl constant camera motion different fps
	google - opengl accellerating camera
	https://gamedev.stackexchange.com/questions/10902/having-a-slight-problem-moving-my-camera-based-on-time-passed-using-glut-opengl
	https://community.khronos.org/t/switching-between-arcball-and-fps-style-camera-rotation/76279/4

*/

class MazharCamera
{
//	const int GLUT_DOWN = 1;
//	const int GLUT_UP = 1;
//	const int GLUT_LEFT_BUTTON = 1;
public:
	enum CameraType 
	{
		ORTHO, FREE
	};

	enum CameraDirection {
		UP, DOWN, LEFT, RIGHT, FORWARD, BACK
	};
	
	MazharCamera();
	~MazharCamera();
	void move(CameraDirection dir, float seconds);
	void Update();
	void GetMatricies(glm::mat4 &P, glm::mat4 &V) const;
	//Setting Functions
	//Changes the camera mode, only three valid modes, Ortho, Free, and Spherical
	void mode(CameraType cam_mode) 
	{
		camera_mode = cam_mode;
	}

	glm::vec3 up() const 
	{
		return camera_up;
	}

	glm::vec3 direction() const
	{
		return 	glm::normalize(camera_look_at - camera_position);
	}

	void position(glm::vec3 pos)
	{
		camera_position = pos;
	}

	glm::vec3 position() const
	{
		return camera_position;
	}

	void lookAt(glm::vec3 pos) 
	{
		camera_look_at = pos;
	}

	glm::vec3 lookAt() const { return camera_look_at;  }

	void FOV(float radians)
	{
		field_of_view = radians;
	}

	void FOV(int degrees)
	{
		field_of_view = static_cast<float>(glm::radians(degrees * 1.0));
	}

	void clipping(float near_clip_distance, float far_clip_distance)
	{
		near_clip = near_clip_distance;
		far_clip = far_clip_distance;
	}

	// void SetDistance(double cam_dist);
	//void SetPos(int button, int state, int screenX, int screenY);
	void cameraScale(float newValue) 
	{ 
		camera_scale = newValue; 
	}
	float cameraScale() const { return camera_scale; }

	//Change the pitch (up, down) for the free camera
	void changePitch(float radians);
	float pitch() const { return camera_pitch; }
	//Given a specific moving direction, the camera will be moved in the appropriate direction
	//For a spherical camera this will be around the look_at point
	//For a free camera a delta will be computed for the direction of movement.
	void aspectRatio(float newValue) 
	{ 
		mAspect = newValue; 
	}
	float aspectRatio() const
	{
		return mAspect;
	}
	// Change yaw(left, right) for the free camera
	void changeHeading(float radiansDelta);
	void roll(float radiansDelta);
private:
	void Reset();
	// This function updates the camera
	// Depending on the current camera mode, the projection and viewport 
	// matricies are computed
	// Then the position and location of the camera is updated


	//Change the heading and pitch of the camera based on the 2d movement of the mouse
	void Move2D(int x, int y);

	//Getting Functions
	CameraType GetMode() const { return camera_mode; }

	CameraType camera_mode = FREE;

	float mAspect = 0.0;
	float field_of_view = glm::radians<float>(45.0);
	float near_clip = 0.0;
	float far_clip = 0.0;

	float camera_scale = 0.5f;
	float camera_heading = 0.0f;
	float camera_pitch = 0.0f;

	float max_pitch_rate = glm::radians<float>(5.0);
	float max_heading_rate = glm::radians<float>(5.0);
	bool move_camera = false;

	glm::vec3 camera_position = glm::vec3(0, 0, 0);
	glm::vec3 camera_position_delta = glm::vec3(0, 0, 0);
	glm::vec3 camera_look_at = glm::vec3(0, 0, 0);
	glm::vec3 camera_direction = glm::vec3(0, 0, 0);

	glm::vec3 camera_up = glm::vec3(0, 1, 0);
	glm::vec3 mouse_position = glm::vec3(0, 1, 0);

	glm::mat4 projection;
	glm::mat4 view;
};
#endif