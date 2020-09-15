#pragma once

#include "../Core/Camera.h"
#include "../Helpers/ErrorHandling.h"

#include "../Cameras/LearnOpenGLCamera.h"


class CameraOpenGL : public Camera
{
	LearnOpenGLCamera mTempRenderTarget;
	LearnOpenGLCamera mCurrentTarget;
	LearnOpenGLCamera* mCurrent = &mCurrentTarget;
public:
	void pushCurrent() override
	{
		// copy current data. No heap involved
		mTempRenderTarget = mCurrentTarget;
		// Point to disposable target
		mCurrent = &mTempRenderTarget;
	}

	void restoreCurrent() override
	{
		// Point back to live target;
		mCurrent = &mCurrentTarget;
	}
	bool processInput(UserInput::AnyInput command, float seconds) override;
	glm::mat4 view() const override { return mCurrent->GetViewMatrix(); }

	glm::mat4 projection() const override
	{
		return glm::perspective(FOV(), 800.0f / 600.0f, 0.1f, 100.0f);
		throw NMSException("Not implemented.");
		return glm::mat4(0.0);
	}
	void lookAt(const glm::vec3& target) override
	{
		glm::vec3 dir = mCurrent->position() - target;
		mCurrent->direction(glm::normalize(dir));
	}
	glm::vec3 up() const override  { return mCurrent->up(); }
	glm::vec3 right() const { return mCurrent->right(); }
	glm::vec3 direction() const override { return mCurrent->direction(); }
	void direction(const glm::vec3& newValue) const override
	{
		mCurrent->direction(glm::normalize(dir));
	}

	void position(const glm::vec3& newValue) override
	{
		mCurrent->position(newValue);
	}
	glm::vec3 position() const override
	{
		return mCurrent->position();
	}
	void FOV(float radians) override
	{
		mCurrent->FOV(radians);
	}
	float FOV() const
	{
		// radians
		return mCurrent->FOV();
	}
	void clipping(float near_clip_distance, float far_clip_distance) override
	{
		mCurrent->clipping(near_clip_distance, far_clip_distance);
	}
	void moveScale(float newValue) override
	{
		mCurrent->moveScale(newValue);
	}
	float moveScale() const override
	{
		return mCurrent->moveScale();
	}

	void mouseScale(float newValue) override
	{
		mCurrent->mouseScale(newValue);
	}
	float mouseScale() const override
	{
		return mCurrent->mouseScale();
	}

	float pitch() const override
	{
		return mCurrent->pitch();
	}

	void pitch(float radians) override
	{
		mCurrent->pitch(radians);
	}
	float yaw() const override
	{
		return mCurrent->yaw();
	}
	void yaw(float radians) override
	{
		mCurrent->yaw(radians);
	}

	void aspectRatio(float newValue) override
	{
		mCurrent->aspectRatio(newValue);
	}
};