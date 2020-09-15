#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class LearnOpenGLCamera
{
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	// Default camera values
	static const float YAW;
	static const float PITCH;
	static const float SPEED;
	static const float SENSITIVITY;
	static const float ZOOM;

	// camera Attributes
	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;
	// euler Angles
	float mYaw;
	float mPitch;
	// camera options
	float mMovementSpeed;
	float mMouseSensitivity;

	// projection attributes
	float mZoom;
	float mNearClip;
	float mFarClip;
	float mAspectRatio;
public:
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// constructor with vectors
	LearnOpenGLCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH)
		: mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED)
		, mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
	{
		mPosition = position;
		mWorldUp = up;
		mYaw = yaw;
		mPitch = pitch;
		updateCameraVectors();
	}
	// constructor with scalar values
	LearnOpenGLCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED)
		, mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
	{
		mPosition = glm::vec3(posX, posY, posZ);
		mWorldUp = glm::vec3(upX, upY, upZ);
		mYaw = yaw;
		mPitch = pitch;
		updateCameraVectors();
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(mPosition, mPosition + mFront, mUp);
	}

	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = mMovementSpeed * deltaTime;

		if (direction == FORWARD)
			mPosition += mFront * velocity;
		if (direction == BACKWARD)
			mPosition -= mFront * velocity;
		if (direction == LEFT)
			mPosition -= mRight * velocity;
		if (direction == RIGHT)
			mPosition += mRight * velocity;
		updateCameraVectors();
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= mMouseSensitivity;
		yoffset *= mMouseSensitivity;

		mYaw += xoffset;
		mPitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (mPitch > 89.0f)
				mPitch = 89.0f;
			if (mPitch < -89.0f)
				mPitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		mZoom -= (float)yoffset;
	}

	void FOV(float radians)
	{
		mZoom = radians;
		if (mZoom < 0.017f) // about one degree
			mZoom = 0.017f;
		if (mZoom > glm::quarter_pi<float>())
			mZoom = glm::quarter_pi<float>();
	}
	float FOV() const { return mZoom; }
	glm::vec3 up() const { return mUp; }
	glm::vec3 direction() const { return mFront; }
	void direction(const glm::vec3& newValue)
	{ 
		mFront = newValue; 
	}
	glm::vec3 right() const { return mRight; }
	void pitch(float delta)
	{
		mPitch += delta;
		updateCameraVectors();
	}
	float pitch() const { return mPitch; }

	void yaw(float delta)
	{
		mYaw += delta;
		updateCameraVectors();
	}
	float yaw() const { return mYaw; }
	glm::vec3 position() const { return mPosition; }
	void position(const glm::vec3& newValue) { mPosition = newValue; }
	void clipping(float nearClip, float farClip)
	{
		mNearClip = nearClip;
		mFarClip = farClip;
	}
	void aspectRatio(float newValue)
	{
		mAspectRatio = newValue;
	}
	float aspectRatio() const { return mAspectRatio; }
	void moveScale(float newValue)
	{
		mMovementSpeed = newValue;
	}
	float moveScale() const { return mMovementSpeed; }
	void mouseScale(float newValue)
	{
		mMouseSensitivity = newValue;
	}
	float mouseScale() const { return mMouseSensitivity; }
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 _front;
		_front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		_front.y = sin(glm::radians(mPitch));
		_front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mFront = glm::normalize(_front);
        // also re-calculate the Right and Up vector
		// normalize the vectors, because their length gets closer to 0 
		// the more you look up or down which results in slower movement.
        mRight = glm::normalize(glm::cross(mFront, mWorldUp));
        mUp    = glm::normalize(glm::cross(mRight, mFront));
    }
};

const float LearnOpenGLCamera::YAW = -90.0f;
const float LearnOpenGLCamera::PITCH = 0.0f;
const float LearnOpenGLCamera::SPEED = 2.5f;
const float LearnOpenGLCamera::SENSITIVITY = 0.1f;
const float LearnOpenGLCamera::ZOOM = 45.0f;
