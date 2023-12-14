#include "CameraMazhar.h"

#include <glm/gtx/string_cast.hpp>

#include <GLFW/glfw3.h>

#include "../Core/Logger.h"
#include "../Core/LogStream.h"

CameraMazhar::CameraMazhar()
{
	mCurrent->Update();
}


bool CameraMazhar::processInput(UserInput::AnyInput input, float seconds)
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
bool isDirectionKey(int key)
{
	return key == UserInput::LogicalOperator::POVForward ||
		key == UserInput::LogicalOperator::POVBack ||
		key == UserInput::LogicalOperator::POVLeft ||
		key == UserInput::LogicalOperator::POVRight;
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
bool CameraMazhar::processKeyboardInput(
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

void CameraMazhar::doProcessKeyboardInput(int userCommand, float seconds)
{
	switch (userCommand)
	{
	case UserInput::LogicalOperator::POVForward:
		mCurrent->move(MazharCamera::CameraDirection::FORWARD, seconds);
		break;
	case UserInput::LogicalOperator::POVBack:
		mCurrent->move(MazharCamera::CameraDirection::BACK, seconds);
		break;
	case UserInput::LogicalOperator::POVLeft:
		mCurrent->move(MazharCamera::CameraDirection::LEFT, seconds);
		break;
	case UserInput::LogicalOperator::POVRight:
		mCurrent->move(MazharCamera::CameraDirection::RIGHT, seconds);
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
	const glm::vec3 v3 = mCurrent->position();
	LogStream(LogStreamLevel::Info) << "Camera position ["
		<< glm::to_string(v3) << "]\n";
}

//bool CameraMazhar::processKeyboardInput1(
//	UserInput::UserCommandCallbackData keyInput, float seconds)
//{
//	bool interestedInKey =
//		keyInput.userCommand == UserInput::BaseUserCommand::POVForward ||
//		keyInput.userCommand == UserInput::BaseUserCommand::POVBack ||
//		keyInput.userCommand == UserInput::BaseUserCommand::POVLeft ||
//		keyInput.userCommand == UserInput::BaseUserCommand::POVRight ||
//		keyInput.userCommand == UserInput::BaseUserCommand::POVPitchDown ||
//		keyInput.userCommand == UserInput::BaseUserCommand::POVPitchUp ||
//		keyInput.userCommand == UserInput::BaseUserCommand::POVYawRight ||
//		keyInput.userCommand == UserInput::BaseUserCommand::POVYawLeft ||
//		keyInput.userCommand == UserInput::BaseUserCommand::POVRollClockwise ||
//		keyInput.userCommand == UserInput::BaseUserCommand::POVRollAntiClockwise;
//	if (interestedInKey)
//	{
//		if (keyInput.action == GLFW_RELEASE)
//		{
//			mCurrentActions.erase(keyInput.userCommand);
//		}
//		else
//		{
//			mCurrentActions.insert(keyInput.userCommand);
//		}
//		LogStream(LogStreamLevel::Info) << "Processing ["
//			<< mCurrentActions.size() << "] actions in Camera UserInput Set\n";
//		std::set<int>::iterator iter = mCurrentActions.begin();
//		while (iter != mCurrentActions.end())
//		{
//			doProcessKeyboardInput(*iter++, seconds);
//		}
//	}
//	return interestedInKey;
//}
//
