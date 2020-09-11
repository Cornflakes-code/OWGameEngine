#pragma once
#include <set>

#include "../Core/Camera.h"
#include "../Helpers/ErrorHandling.h"

#include "OW_Camera.h"

class CameraOW : public Camera
{
	OW_Camera mTempRenderTarget;
	OW_Camera mCurrentTarget;
	OW_Camera* mCurrent = &mCurrentTarget;

public:
	CameraOW();
	void update()
	{
		//mCurrent->Update();
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
		return mCurrent->view();
	}
	glm::mat4 projection() const override
	{
		return mCurrent->projection();
	}
	glm::vec3 lookAt() const 
	{ 
		return mCurrent->lookAt();
	}
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
		mCurrent->moveScale(newValue);
	}
	float moveScale() const override
	{
		return mCurrent->moveScale();
	}
	void mouseScale(float newValue) override
	{
		throw NMSException("Not implemented");
		//mCurrent->cameraScale(newValue);
	}
	float mouseScale() const override
	{
		throw NMSException("Not implemented");
		return 0.0;// mCurrent->cameraScale();
	}
	void pitch(float radians) override
	{
		mCurrent->pitch(radians);
	}
	void yaw(float radians) override
	{
		mCurrent->yaw(radians);
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
private:
	bool processKeyboardInput(
		UserInput::UserCommandCallbackData keyInput, float seconds);

	void doProcessKeyboardInput(int userCommand, float seconds);
	std::set<int> mCurrentActions;
	int mDirection = 0;
	int mRoll = 0;
	int mPitch = 0;
	int mYaw = 0;

};