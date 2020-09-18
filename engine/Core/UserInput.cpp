#include "UserInput.h"

#include "GLApplication.h"
#include "../Helpers/ErrorHandling.h"

// AnyKey must be mapped to a non printable key
int UserInput::AnyKey = 0;

UserInput::UserInput()
{
	// Key mapping taken from keymapping for Elit Dangerous at
	// https://guides.gamepressure.com/elitedangerous/guide.asp?ID=29578
	//
	// The aim is for the camera to fly like a spaceship. Left and right are the 
	// strafing options and may be removed. Yaw, Pitch and forward/back provide 
	// the main movement
	mBaseKeyMapping[{GLFW_KEY_W, InputMods::NoMod}] = BaseUserCommand::POVForward;
	mBaseKeyMapping[{GLFW_KEY_S, InputMods::NoMod}] = BaseUserCommand::POVBack;

	mBaseKeyMapping[{GLFW_KEY_A, InputMods::NoMod}] = BaseUserCommand::POVYawLeft;
	mBaseKeyMapping[{GLFW_KEY_D, InputMods::NoMod}] = BaseUserCommand::POVYawRight;

	mBaseKeyMapping[{GLFW_KEY_A, InputMods::Shift}] = BaseUserCommand::POVLeft;
	mBaseKeyMapping[{GLFW_KEY_D, InputMods::Shift}] = BaseUserCommand::POVRight;

	mBaseKeyMapping[{GLFW_KEY_E, InputMods::NoMod}] = BaseUserCommand::POVPitchDown;
	mBaseKeyMapping[{GLFW_KEY_Q, InputMods::NoMod}] = BaseUserCommand::POVPitchUp;

	mBaseKeyMapping[{GLFW_KEY_Q, InputMods::Shift}] = BaseUserCommand::POVRollAntiClockwise;
	mBaseKeyMapping[{GLFW_KEY_E, InputMods::Shift}] = BaseUserCommand::POVRollClockwise;

	mBaseKeyMapping[{GLFW_KEY_F5, InputMods::NoMod}] = BaseUserCommand::Save;
	mBaseKeyMapping[{GLFW_KEY_F6, InputMods::NoMod}] = BaseUserCommand::Restore;
	mBaseKeyMapping[{GLFW_KEY_F7, InputMods::NoMod}] = BaseUserCommand::RecordingStart;
	mBaseKeyMapping[{GLFW_KEY_ESCAPE, InputMods::NoMod}] = BaseUserCommand::OptionsScreen;
	mBaseKeyMapping[{GLFW_KEY_SPACE, InputMods::NoMod}] = BaseUserCommand::Accept;
}

void UserInput::init(GLApplication* app)
{
	app->addPointingDeviceListener([this](GLFWwindow* window, int button, int action, int mods)
	{
		doMouseClick(window, button, action, mods);
	});

	app->addKeyboardListener([this](unsigned int codepoint,
		int key, int scancode, int action, int mods)
	{
		doKeyPressCallback(codepoint, key, scancode, action, mods);
	});

	app->addWindowResizeListener([this](GLApplication::WindowResizeType resizeType, glm::ivec2 newSize)
	{
		if (resizeType == GLApplication::WindowResizeType::FrameBuffer)
		{
			mFrameBuffer = newSize;
		}
		else if (resizeType == GLApplication::WindowResizeType::WindowResize)
		{
			mWindowSize = newSize;
		}
	});
}

UserInput::PointingDeviceAction getMouseButton(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		return UserInput::PointingDeviceAction::LeftMouseButtonClick;
	case GLFW_MOUSE_BUTTON_RIGHT:
		return UserInput::PointingDeviceAction::RightMouseButtonClick;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		return UserInput::PointingDeviceAction::MiddleMouseButtonClick;
	default:
		return UserInput::PointingDeviceAction::Unknown;
	}
}

void UserInput::doMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		// https://stackoverflow.com/questions/45796287/screen-coordinates-to-world-coordinates
		// https://stackoverflow.com/questions/11277501/how-to-recover-view-space-position-given-view-space-depth-value-and-ndc-xy?noredirect=1&lq=1
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec2 screen_pos = glm::vec2(xpos, ypos);
		glm::vec2 pixel_pos 
			= screen_pos * glm::vec2(mFrameBuffer.x, mFrameBuffer.y) / 
						  glm::vec2(mWindowSize.x, mWindowSize.y);
		// shift to GL's center convention
		pixel_pos = pixel_pos + glm::vec2(0.5f, 0.5f); 

		PointingDeviceCallbackData data;
		data.pos = glm::vec3(pixel_pos.x, mWindowSize.y - pixel_pos.y, 0.0f);

		glReadPixels(static_cast<GLint>(xpos), static_cast<GLint>(ypos), 1, 1,
				GL_DEPTH_COMPONENT, GL_FLOAT, &data.pos.z);
		data.action = getMouseButton(button);
		data.mods = mods;

		for (auto& cb : mPointingDeviceCallbacks)
		{
			cb(data);
		}
	}
}

void UserInput::doKeyPressCallback(unsigned int codepoint, 
					int key, int /*scancode*/, int action, int mods)
{
	UserCommandCallbackData data;
	if (codepoint)
	{
		// If you wish to offer regular text input, set a character callback.
		// https://www.glfw.org/docs/3.3/input_guide.html
		// https://www.glfw.org/docs/3.0/group__input.html
	}
	else
	{
		if (key < 32)
			return;
		dumpMessage(std::stringstream() << "data.key["
			<< data.key << "] action[" << action << "]\n", NMSErrorLevel::NMSInfo);
		data.action = action;
		data.key = key;
		data.mods = mods;
		data.userCommand = userCommand(data);
		if (data.userCommand == BaseUserCommand::NoCommand)
			return;
		for (auto& cb : mUserCommandCallbacks)
		{
			cb(data);
		}
	}
}

int UserInput::userCommand(const UserCommandCallbackData& data)
{
	auto iter = mBaseKeyMapping.begin();
	while (iter != mBaseKeyMapping.end())
	{
		if (iter->first.userCommand == data.key)
		{
			if (iter->first.keyMod == InputMods::NoMod && data.mods == 0)
				return iter->second;
			if (iter->first.keyMod & data.mods)
				return iter->second;
		}
		++iter;
	}
	return BaseUserCommand::NoCommand; 
}

/*
GLFWAPI GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback(GLFWwindow* window, GLFWwindowcontentscalefun callback);
GLFWAPI GLFWcharmodsfun glfwSetCharModsCallback(GLFWwindow* window, GLFWcharmodsfun callback);
GLFWAPI GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun callback);
*/