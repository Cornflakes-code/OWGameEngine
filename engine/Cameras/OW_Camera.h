#pragma once
#include <iostream>

#include <glm/glm.hpp>

#include "../Core/ErrorHandling.h"

#include <glm/gtx/rotate_vector.hpp>

/*
	A first pass at a Camera. Wrapped by CameraOW. Needs to be replaced
*/
class OW_Camera
{
	glm::vec3 mPosition = glm::vec3(0, 0, 0);
	glm::vec3 mDirection = glm::vec3(0, 0, 0);
	glm::vec3 mUp = glm::vec3(0, 1, 0);
	float mNearClip = 0.0;
	float mFarClip = 0.0;
	float mAspect = 0.0;
	float mFieldOfView = glm::radians<float>(45.0);
	float mMoveScale = 0.5f;
	float mMouseScale = 0.5f;
public:
	enum CameraDirection
	{
		FORWARD,
		BACK,
		LEFT,
		RIGHT
	};

	glm::mat4 view() const
	{
		return glm::lookAt(mPosition, mPosition + mDirection, mUp);
	}
	glm::mat4 projection() const
	{
		return glm::perspective(mFieldOfView, mAspect, mNearClip, mFarClip);

	}
	void lookAt(const glm::vec3& target)
	{
		glm::vec3 temp = target * 1.0f;
		mDirection = glm::normalize(temp - mPosition);
	}
	glm::vec3 lookAt() const
	{
		return mPosition + mDirection;
	}
	glm::vec3 up() const { return mUp; }
	glm::vec3 right() const
	{
		return glm::normalize(glm::cross(mDirection, mUp));
	}
	glm::vec3 direction() { return mDirection; }

	void position(const glm::vec3& pos)
	{
		mPosition = pos;
	}
	glm::vec3 position() const { return mPosition; }

	void FOV(float radians) 
	{ 
		mFieldOfView = radians; 
		if (mFieldOfView < 0.017f) // about one degree
			mFieldOfView = 0.017f;

		if (mFieldOfView > glm::quarter_pi<float>())
			mFieldOfView = glm::quarter_pi<float>();
	}

	void clipping(float near_clip_distance, float far_clip_distance)
	{
		mNearClip = near_clip_distance;
		mFarClip = far_clip_distance;
	}

	void moveScale(float newValue)
	{
		mMoveScale = newValue;
	}

	float moveScale() const { return mMoveScale; }

	void mouseScale(float newValue)
	{
		mMouseScale = newValue;
	}

	float mouseScale() const { return mMouseScale; }

	void move(CameraDirection direction, float deltaTime)
	{
		float velocity = mMoveScale * deltaTime;
		switch (direction)
		{
		case FORWARD:
			mPosition += mDirection * velocity;
			break;
		case BACK:
			mPosition -= mDirection * velocity;
			break;
		case RIGHT:
			mPosition += right() * velocity;
			break;
		case LEFT:
			mPosition -= right() * velocity;
			break;
		default:
			throw NMSException(std::stringstream()
				<< "Unhandled direction[" << direction << "] in OWCamera.\n");
		}
	}
	void pitch(float radians)
	{
		glm::vec3 _right = right();
		mDirection  = glm::normalize(glm::rotate(mDirection, -radians, _right));
		mUp = glm::normalize(glm::rotate(mUp, -radians, _right));
	}

	void roll(float radians)
	{
		mUp = glm::normalize(glm::rotate(mUp, radians, mDirection));
	}

	void yaw(float radians)
	{
		mDirection = glm::normalize(glm::rotate(mDirection, radians, mUp));
	}

	void aspectRatio(float newValue) 
	{ 
		mAspect = newValue; 
	}
	float aspectRatio() const { return mAspect; }
};