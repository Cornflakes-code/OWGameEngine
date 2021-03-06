#include "UserInput.h"

#include "GLApplication.h"
#include "ErrorHandling.h"
#include "LogStream.h"

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

void UserInput::pointingDevice(void* window, int button, int action, 
				int mods, const glm::vec3& pos)
{
	if (action == GLFW_PRESS)
	{
		PointingDeviceCallbackData data;
		data.pos = pos;
		data.action = getMouseButton(button);
		data.mods = mods;

		for (auto& cb : mPointingDeviceCallbacks)
		{
			cb.first(data);
		}
	}
}

void UserInput::cursorPosition(void* window, double x, double y)
{
	PointingDeviceCallbackData data;
	data.action = UserInput::PointingDeviceAction::MouseMove;
	data.pos = glm::vec3(x, y, 0);
	for (auto& cb : mPointingDeviceCallbacks)
	{
		cb.first(data);
	}
}


void UserInput::keyboard(unsigned int codepoint,
						int key, int OW_UNUSED(scancode), int action, int mods)
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
		LogStream(LogStreamLevel::Info) << "data.key["
			<< data.key << "] action[" << action << "]\n";
		data.action = action;
		data.key = key;
		data.mods = mods;
		data.userCommand = userCommand(data);
		if (data.userCommand == BaseUserCommand::NoCommand)
			return;
		for (auto& cb : mUserCommandCallbacks)
		{
			cb.first(data);
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

void UserInput::windowResize(void* window, const glm::ivec2& widthHeight)
{
	UserCommandCallbackData data;
	data.action = 0;
	data.key = 0;
	data.mods = 0;
	data.userCommand = UserInput::WindowResize;
	for (auto& cb : mUserCommandCallbacks)
	{
		cb.first(data);
	}
}

template <class T>
void removeListener(std::vector<std::pair<T, size_t>> v, 
				const ListenerHelper* helper)
{
	if (helper)
	{
		auto iter = v.begin();
		while (iter != v.end())
		{
			if (iter->second == helper->mUniqueId)
			{
				v.erase(iter);
				return;
			}
			++iter;
		}
	}
}

void UserInput::removeUserCommandListener(const ListenerHelper* helper)
{
	removeListener(mPointingDeviceCallbacks, helper);
}

void UserInput::removePointingDeviceListener(const ListenerHelper* helper)
{
	removeListener(mPointingDeviceCallbacks, helper);
}

void UserInput::removeWindowResizeListener(const ListenerHelper* helper)
{
	removeListener(mWindowResizeCallbacks, helper);
}

void UserInput::closeWindow(void* window)
{
	UserCommandCallbackData data;
	data.action = 0;
	data.key = 0;
	data.mods = 0;
	data.userCommand = UserInput::WindowClose;
	for (auto& cb : mUserCommandCallbacks)
	{
		cb.first(data);
	}
}

/*
GLFWAPI GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback(GLFWwindow* window, GLFWwindowcontentscalefun callback);
GLFWAPI GLFWcharmodsfun glfwSetCharModsCallback(GLFWwindow* window, GLFWcharmodsfun callback);
GLFWAPI GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun callback);
*/