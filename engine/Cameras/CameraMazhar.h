#pragma once
#include <set>

#include "../Cameras/Camera.h"

#include "CameraMazharImp.h"

/*
	Implements the Camera interface with class MazharCamera providing the functionality.
*/
class CameraMazhar : public Camera, public ListenerHelper
{
	CameraMazharImp mTempRenderTarget;
	CameraMazharImp mCurrentTarget;
	CameraMazharImp* mCurrent = &mCurrentTarget;

public:
	CameraMazhar();
	void bindResize(UserInput* app) override;
	void update()
	{
		mCurrent->Update();
	}
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
	bool processInput(UserInput::AnyInput input, float seconds) override;
	glm::mat4 view() const override
	{
		glm::mat4 p;
		glm::mat4 v;
		mCurrent->GetMatricies(p, v);
		return v;
	}
	glm::mat4 projection() const override
	{
		glm::mat4 p;
		glm::mat4 v;
		mCurrent->GetMatricies(p, v);
		return p;
	}
	glm::vec3 lookAt() const { return mCurrent->lookAt(); }
	void lookAt(const glm::vec3& target) override
	{
		mCurrent->lookAt(target);
	}
	glm::vec3 up() const override
	{
		return mCurrent->up();
	}
	glm::vec3 right() const override
	{ 
		return glm::normalize(glm::cross(mCurrent->direction(), mCurrent->up()));
	}
	glm::vec3 front() const override
	{
		glm::vec3 front;
		front.x = cos(glm::radians(mCurrent->yaw())) * cos(glm::radians(mCurrent->pitch()));
		front.y = sin(glm::radians(mCurrent->pitch()));
		front.z = sin(glm::radians(mCurrent->yaw())) * cos(glm::radians(mCurrent->pitch()));
		front = glm::normalize(front);
		return front;
	}

	glm::vec3 direction() const override
	{
		return mCurrent->direction();
	}
	void position(const glm::vec3& pos) override
	{
		mCurrent->position(pos);
	}
	glm::vec3 position() const override
	{
		return mCurrent->position();
	}
	void FOV(float radians) override
	{
		mCurrent->FOV(radians);
	}
	void clipping(float near_clip_distance, float far_clip_distance) override
	{
		mCurrent->clipping(near_clip_distance, far_clip_distance);
	}
	void moveScale(float newValue) override
	{
		mCurrent->cameraScale(newValue);
	}
	float moveScale() const override
	{
		return mCurrent->cameraScale();
	}
	void mouseScale(float OW_UNUSED(newValue)) override;
	float mouseScale() const override;
	void pitch(float radians) override
	{
		mCurrent->changePitch(radians);
	}
	void yaw(float radians) override
	{
		mCurrent->changeHeading(radians);
	}
	void roll(float radians) override
	{
		mCurrent->roll(radians);
	}
	void aspectRatio(float newValue) override
	{
		mCurrent->aspectRatio(newValue);
	}
	float aspectRatio() const override
	{
		return mCurrent->aspectRatio();
	}
	BoundingFrustum createFrustum(float aspect, float fovY,
		float zNear, float zFar) const override;
private:
	void resizeCallback(void* OW_UNUSED(window), glm::ivec2 dimensions)
	{
		// There is a lot of interaction here between:
		// 1. mAspectRatio.
		// 2. call to setViewport in Movie.cpp
		// 3. camera projection call passed to render.
		mCurrent->aspectRatio(dimensions.x / (dimensions.y * 1.0f));
	}
	bool processKeyboardInput(
		UserInput::UserCommandCallbackData keyInput, float seconds);
	//bool processKeyboardInput1(
	//	UserInput::UserCommandCallbackData keyInput, float seconds);

	void doProcessKeyboardInput(int userCommand, float seconds);
	std::set<int> mCurrentActions;
	int mDirection = 0;
	int mRoll = 0;
	int mPitch = 0;
	int mYaw = 0;
};