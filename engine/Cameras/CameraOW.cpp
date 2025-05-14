#include "CameraOW.h"

#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

#include "../Core/GlobalSettings.h"
#include "../Core/ErrorHandling.h"
#include "../Core/Logger.h"
#include "../Core/LogStream.h"

CameraOW::CameraOW()
{
}

void CameraOW::bindResize(UserInput* app)
{
	auto cb = std::bind(&CameraOW::resizeCallback, this,
		std::placeholders::_1, std::placeholders::_2);
	app->addWindowResizeListener(cb, this);
}

glm::vec3 CameraOW::front() const 
{
	throw NMSException(std::stringstream()
		<< "Unimplemented function [CameraOW::front()].");
	glm::vec3 front;
	//front.x = cos(glm::radians(mCurrent->yaw())) * cos(glm::radians(mCurrent->pitch()));
	//front.y = sin(glm::radians(mCurrent->pitch()));
	//front.z = sin(glm::radians(mCurrent->yaw())) * cos(glm::radians(mCurrent->pitch()));
	//front = glm::normalize(front);
	//return front;
}

bool CameraOW::processInput(UserInput::AnyInput input, float seconds)
{
	if (input.inputType == UserInput::AnyInputType::KeyPress)
	{
		return processKeyboardInput(input.keyInput, seconds);
	}
	return false;
}

/*
	Camera movement should be able to handle multiple keys
	pressed at the same time. Store key presses in a set on GLFW_PRESS
	and GLFW_REPEAT and remove them on GLFW_RELEASE
*/
static bool isDirectionKey(int key)
{
	return key == UserInput::LogicalOperator::POVForward ||
		key == UserInput::LogicalOperator::POVBack ||
		key == UserInput::LogicalOperator::POVLeft ||
		key == UserInput::LogicalOperator::POVRight ||
		key == UserInput::LogicalOperator::POVDown ||
		key == UserInput::LogicalOperator::POVUp;
}

static bool isYawKey(int key)
{
	return key == UserInput::LogicalOperator::POVYawRight ||
		key == UserInput::LogicalOperator::POVYawLeft;
}

static bool isPitchKey(int key)
{
	return key == UserInput::LogicalOperator::POVPitchUp ||
		key == UserInput::LogicalOperator::POVPitchDown;
}

static bool isRollKey(int key)
{
	return key == UserInput::LogicalOperator::POVRollClockwise ||
		key == UserInput::LogicalOperator::POVRollAntiClockwise;
}

typedef std::function<bool(int)> KeyTest;

/*
	Use Case: if the user releases the held down key then remove it
*/
static bool conditionalRemove(KeyTest condition, int key, int& keyStore)
{
	bool foundKey = condition(key);
	if (foundKey)
		if (keyStore == key)
			keyStore = 0;
	return foundKey;
}

/*
	Use Case: If the user is holding down pitchDown and presses
	pitchUp then we assume he wants to change to pitchUp immediately.
	Use Case: If the user is not holding down any pitch key, and
	presses a pitch key, change to it immediately
*/
static bool conditionalAdd(KeyTest condition, int key, int& keyStore)
{
	bool foundKey = condition(key);
	if (foundKey)
		keyStore = key;
	return foundKey;
}
/*
	For example, the user can press yaw and roll at the same time
	but pressing rollLeft and rollRight. This applies to all four
	action groups (direction/Yaw/Pitch/Roll
*/
bool CameraOW::processKeyboardInput(
	UserInput::UserCommandCallbackData keyInput, float seconds)
{
	// https://stackoverflow.com/questions/46631814/handling-multiple-keys-input-at-once-with-glfw/48286375
	bool foundKey = false;
	if (keyInput.action == GLFW_RELEASE)
	{
		// as soon as a key is found then no need to test further
		foundKey =
			conditionalRemove(isDirectionKey, keyInput.userCommand, mDirection) ||
			conditionalRemove(isPitchKey, keyInput.userCommand, mPitch) ||
			conditionalRemove(isRollKey, keyInput.userCommand, mRoll) ||
			conditionalRemove(isYawKey, keyInput.userCommand, mYaw);
	}
	else
	{
		foundKey =
			conditionalAdd(isDirectionKey, keyInput.userCommand, mDirection) ||
			conditionalAdd(isRollKey, keyInput.userCommand, mRoll) ||
			conditionalAdd(isPitchKey, keyInput.userCommand, mPitch) ||
			conditionalAdd(isYawKey, keyInput.userCommand, mYaw);
	}
	LogStream(LogStreamLevel::Info) << "processKeyboardInput\n";

	// Does order of these calls matter????
	// Also there is a delay. Maybe caused by the call to processUserInput
	// is in the wrong place in Movie::run.

	if (mPitch)
	{
		doProcessKeyboardInput(mPitch, seconds);
		LogStream(LogStreamLevel::Info) << "Processing pitch\n";
	}
	if (mRoll)
	{
		doProcessKeyboardInput(mRoll, seconds);
		LogStream(LogStreamLevel::Info) << "Processing roll\n";
	}
	if (mYaw)
	{
		doProcessKeyboardInput(mYaw, seconds);
		LogStream(LogStreamLevel::Info) << "Processing yaw\n";
	}
	if (mDirection)
	{
		doProcessKeyboardInput(mDirection, seconds);
		LogStream(LogStreamLevel::Info) << "Processing direction\n";
	}
	return foundKey;
}

void CameraOW::doProcessKeyboardInput(int userCommand, float seconds)
{
	switch (userCommand)
	{
	case UserInput::LogicalOperator::POVForward:
		mCurrent->move(CameraOWImp::CameraDirection::FORWARD, seconds);
		break;
	case UserInput::LogicalOperator::POVBack:
		mCurrent->move(CameraOWImp::CameraDirection::BACK, seconds);
		break;
	case UserInput::LogicalOperator::POVLeft:
		mCurrent->move(CameraOWImp::CameraDirection::LEFT, seconds);
		break;
	case UserInput::LogicalOperator::POVRight:
		mCurrent->move(CameraOWImp::CameraDirection::RIGHT, seconds);
		break;
	case UserInput::LogicalOperator::POVDown:
		mCurrent->move(CameraOWImp::CameraDirection::DOWN, seconds);
		break;
	case UserInput::LogicalOperator::POVUp:
		mCurrent->move(CameraOWImp::CameraDirection::UP, seconds);
		break;
	case UserInput::LogicalOperator::POVPitchDown:
		pitch(glm::radians(-1.0f));
		break;
	case UserInput::LogicalOperator::POVPitchUp:
		pitch(glm::radians(1.0f));
		break;
	case UserInput::LogicalOperator::POVYawRight:
		yaw(glm::radians(-1.0f));
		break;
	case UserInput::LogicalOperator::POVYawLeft:
		yaw(glm::radians(1.0f));
		break;
	case UserInput::LogicalOperator::POVRollClockwise:
		mCurrent->roll(glm::radians(-1.0f));
		break;
	case UserInput::LogicalOperator::POVRollAntiClockwise:
		mCurrent->roll(glm::radians(1.0f));
		break;
	default:
		throw NMSException(std::stringstream()
			<< "Invalid keyboard Input ["
			<< userCommand << "] stored in Camera.\n");
	}
}

void CameraOW::mouseScale(float OW_UNUSED(newValue)) 
{
	throw NMSException("Not implemented");
	//mCurrent->cameraScale(newValue);
}

float CameraOW::mouseScale() const 
{
	throw NMSException("Not implemented");
	return 0.0;// mCurrent->cameraScale();
}

BoundingFrustum CameraOW::createFrustum(float aspect, float fovY,
	float zNear, float zFar) const
{
	BoundingFrustum frustum;
	const float halfVSide = zFar * tanf(fovY * .5f);
	const float halfHSide = halfVSide * aspect;
	const glm::vec3 frontMultFar = zFar * front();

	glm::vec3 _front = front();
	frustum.nearFace = { position() + zNear * _front, _front };
	frustum.farFace = { position() + frontMultFar, -_front };
	frustum.rightFace = { position(),
							glm::cross(frontMultFar - right() * halfHSide, up()) };
	frustum.leftFace = { position(),
							glm::cross(up(),frontMultFar + right() * halfHSide) };
	frustum.topFace = { position(),
							glm::cross(right(), frontMultFar - up() * halfVSide) };
	frustum.bottomFace = { position(),
							glm::cross(frontMultFar + up() * halfVSide, right()) };

	return frustum;
}
