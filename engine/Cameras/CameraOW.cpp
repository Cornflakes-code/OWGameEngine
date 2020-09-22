#include "CameraOW.h"

#include <Core/GLApplication.h>
#include "../Helpers/Logger.h"
#include <glm/gtx/string_cast.hpp>

CameraOW::CameraOW()
{
	if (globals && globals->application())
	{
		auto cb = std::bind(&CameraOW::resizeCallback, this,
			std::placeholders::_1, std::placeholders::_2);
		globals->application()->addWindowResizeListener(cb, this);
	}
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
	return key == UserInput::BaseUserCommand::POVForward ||
		key == UserInput::BaseUserCommand::POVBack ||
		key == UserInput::BaseUserCommand::POVLeft ||
		key == UserInput::BaseUserCommand::POVRight;
}

static bool isYawKey(int key)
{
	return key == UserInput::BaseUserCommand::POVYawRight ||
		key == UserInput::BaseUserCommand::POVYawLeft;
}

static bool isPitchKey(int key)
{
	return key == UserInput::BaseUserCommand::POVPitchUp ||
		key == UserInput::BaseUserCommand::POVPitchDown;
}

static bool isRollKey(int key)
{
	return key == UserInput::BaseUserCommand::POVRollClockwise ||
		key == UserInput::BaseUserCommand::POVRollAntiClockwise;
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
	dumpMessage("processKeyboardInput\n", NMSErrorLevel::NMSInfo);

	// Does order of these calls matter????
	// Also there is a delay. Maybe caused by the call to processUserInput
	// is in the wrong place in Movie::run.

	if (mPitch)
	{
		doProcessKeyboardInput(mPitch, seconds);
		dumpMessage("Processing pitch\n", NMSErrorLevel::NMSInfo);
	}
	if (mRoll)
	{
		doProcessKeyboardInput(mRoll, seconds);
		dumpMessage("Processing roll\n", NMSErrorLevel::NMSInfo);
	}
	if (mYaw)
	{
		doProcessKeyboardInput(mYaw, seconds);
		dumpMessage("Processing yaw\n", NMSErrorLevel::NMSInfo);
	}
	if (mDirection)
	{
		doProcessKeyboardInput(mDirection, seconds);
		dumpMessage("Processing direction\n", NMSErrorLevel::NMSInfo);
	}
	return foundKey;
}

void CameraOW::doProcessKeyboardInput(int userCommand, float seconds)
{
	switch (userCommand)
	{
	case UserInput::BaseUserCommand::POVForward:
		mCurrent->move(OW_Camera::CameraDirection::FORWARD, seconds);
		break;
	case UserInput::BaseUserCommand::POVBack:
		mCurrent->move(OW_Camera::CameraDirection::BACK, seconds);
		break;
	case UserInput::BaseUserCommand::POVLeft:
		mCurrent->move(OW_Camera::CameraDirection::LEFT, seconds);
		break;
	case UserInput::BaseUserCommand::POVRight:
		mCurrent->move(OW_Camera::CameraDirection::RIGHT, seconds);
		break;
	case UserInput::BaseUserCommand::POVPitchDown:
		pitch(glm::radians(-1.0f));
		break;
	case UserInput::BaseUserCommand::POVPitchUp:
		pitch(glm::radians(1.0f));
		break;
	case UserInput::BaseUserCommand::POVYawRight:
		yaw(glm::radians(-1.0f));
		break;
	case UserInput::BaseUserCommand::POVYawLeft:
		yaw(glm::radians(1.0f));
		break;
	case UserInput::BaseUserCommand::POVRollClockwise:
		mCurrent->roll(glm::radians(-1.0f));
		break;
	case UserInput::BaseUserCommand::POVRollAntiClockwise:
		mCurrent->roll(glm::radians(1.0f));
		break;
	default:
		throw NMSException(std::stringstream()
			<< "Invalid keyboard Input ["
			<< userCommand << "] stored in Camera.\n");
	}
	const glm::vec3 v3 = mCurrent->position();
	dumpMessage(std::stringstream() << "Camera position ["
		<< glm::to_string(v3) << "]\n",
		NMSErrorLevel::NMSInfo);
}
