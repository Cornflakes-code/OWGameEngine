#include "CameraLearnOpenGL.h"

#include "../Helpers/ErrorHandling.h"

bool CameraOpenGL::processInput(UserInput::AnyInput input, float seconds)
{
	// send elsewhere (camera? MacroMachine?) for processing
	bool handled = false;
	if (input.command == UserInput::BaseUserCommand::POVForward)
	{
		mCurrent->ProcessKeyboard(LearnOpenGLCamera::Camera_Movement::FORWARD, seconds);
		handled = true;
		dumpMessage("forward", NMSErrorLevel::NMSInfo);
	}
	else if (input.command == UserInput::BaseUserCommand::POVBack)
	{
		mCurrent->ProcessKeyboard(LearnOpenGLCamera::Camera_Movement::BACKWARD, seconds);
		handled = true;
		dumpMessage("back", NMSErrorLevel::NMSInfo);
	}
	else if (input.command == UserInput::BaseUserCommand::POVLeft)
	{
		mCurrent->ProcessKeyboard(LearnOpenGLCamera::Camera_Movement::LEFT, seconds);
		handled = true;
		dumpMessage("left", NMSErrorLevel::NMSInfo);
	}
	else if (input.command == UserInput::BaseUserCommand::POVRight)
	{
		mCurrent->ProcessKeyboard(LearnOpenGLCamera::Camera_Movement::RIGHT, seconds);
		handled = true;
		dumpMessage("right", NMSErrorLevel::NMSInfo);
	}
	else if (input.command == UserInput::BaseUserCommand::POVPitchDown)
	{
		//mCurrent->changePitch(-1.0);
		handled = false;
		dumpMessage("pitch down", NMSErrorLevel::NMSInfo);
	}
	else if (input.command == UserInput::BaseUserCommand::POVPitchUp)
	{
		//mCurrent->changePitch(1.0);
		handled = false;
		dumpMessage("pitch up", NMSErrorLevel::NMSInfo);
	}
	return handled;
}
