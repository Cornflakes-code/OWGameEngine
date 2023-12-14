#include "UserInput.h"

#include "GLApplication.h"
#include "ErrorHandling.h"
#include "LogStream.h"

// AnyKey must be mapped to a non printable key
int UserInput::AnyKey = 0;

UserInput::UserInput()
{
	/*
	// Key mapping taken from keymapping for Elite Dangerous at
	// https://guides.gamepressure.com/elitedangerous/guide.asp?ID=29578
	//
	// The aim is for the camera to fly like a spaceship. Left and right are the 
	// strafing options and may be removed. Yaw, Pitch and forward/back provide 
	// the main movement
	mBaseKeyMapping[{GLFW_KEY_W, InputMod::NoMod}] = BaseUserCommand::POVForward;
	mBaseKeyMapping[{GLFW_KEY_S, InputMod::NoMod}] = BaseUserCommand::POVBack;

	mBaseKeyMapping[{GLFW_KEY_A, InputMod::NoMod}] = BaseUserCommand::POVYawLeft;
	mBaseKeyMapping[{GLFW_KEY_D, InputMod::NoMod}] = BaseUserCommand::POVYawRight;

	mBaseKeyMapping[{GLFW_KEY_A, InputMod::Shift}] = BaseUserCommand::POVLeft;
	mBaseKeyMapping[{GLFW_KEY_D, InputMod::Shift}] = BaseUserCommand::POVRight;

	mBaseKeyMapping[{GLFW_KEY_E, InputMod::NoMod}] = BaseUserCommand::POVPitchDown;
	mBaseKeyMapping[{GLFW_KEY_Q, InputMod::NoMod}] = BaseUserCommand::POVPitchUp;

	mBaseKeyMapping[{GLFW_KEY_Q, InputMod::Shift}] = BaseUserCommand::POVRollAntiClockwise;
	mBaseKeyMapping[{GLFW_KEY_E, InputMod::Shift}] = BaseUserCommand::POVRollClockwise;

	mBaseKeyMapping[{GLFW_KEY_F5, InputMod::NoMod}] = BaseUserCommand::Save;
	mBaseKeyMapping[{GLFW_KEY_F6, InputMod::NoMod}] = BaseUserCommand::Restore;
	mBaseKeyMapping[{GLFW_KEY_F7, InputMod::NoMod}] = BaseUserCommand::RecordingStart;
	mBaseKeyMapping[{GLFW_KEY_ESCAPE, InputMod::NoMod}] = BaseUserCommand::OptionsScreen;
	mBaseKeyMapping[{GLFW_KEY_SPACE, InputMod::NoMod}] = BaseUserCommand::Accept;
	*/
	createLogicalOperatorStringMap();
	createInputModStringMap();
}

void UserInput::addMapping(UserInput::LogicalOperator buc, const std::string& s)
{
	mLogicalOperatorStringMap.emplace(buc, s);
}

void UserInput::createLogicalOperatorStringMap()
{
	addMapping(LogicalOperator::NoCommand, "NoCommand");
	addMapping(LogicalOperator::Save, "Save");
	addMapping(LogicalOperator::POVForward, "POVForward");
	addMapping(LogicalOperator::POVBack, "POVBack");
	addMapping(LogicalOperator::POVLeft, "POVLeft");
	addMapping(LogicalOperator::POVRight, "POVRight");
	addMapping(LogicalOperator::POVPitchDown, "POVPitchDown");
	addMapping(LogicalOperator::POVPitchUp, "POVPitchUp");
	addMapping(LogicalOperator::POVYawLeft, "POVYawLeft");
	addMapping(LogicalOperator::POVYawRight, "POVYawRight");
	addMapping(LogicalOperator::POVRollClockwise, "POVRollClockwise");
	addMapping(LogicalOperator::POVRollAntiClockwise, "POVRollAntiClockwise");
	addMapping(LogicalOperator::Restore, "Restore");
	addMapping(LogicalOperator::RecordingStart, "RecordingStart");
	addMapping(LogicalOperator::RecordingEnd, "RecordingEnd");
	addMapping(LogicalOperator::PlaybackStart, "PlaybackStart");
	addMapping(LogicalOperator::PlaybackEnd, "PlaybackEnd");
	addMapping(LogicalOperator::OptionsScreen, "OptionsScreen");
	addMapping(LogicalOperator::RopeScreen, "RopeScreen");
	addMapping(LogicalOperator::Accept, "Accept");
	addMapping(LogicalOperator::WindowResize, "WindowResize");
	addMapping(LogicalOperator::WindowClose, "WindowClose");
	addMapping(LogicalOperator::WindowClose, "WindowClose");
	addMapping(LogicalOperator::Special1, "Special1");
	addMapping(LogicalOperator::Special2, "Special2");
	addMapping(LogicalOperator::Special3, "Special3");
	addMapping(LogicalOperator::Special4, "Special4");
	addMapping(LogicalOperator::Special5, "Special5");
	addMapping(LogicalOperator::Special6, "Special6");
}

void UserInput::createInputModStringMap()
{
	mInputModsStringMap.emplace(InputMod::Alt, "Alt");
	mInputModsStringMap.emplace(InputMod::CapsLock, "CapsLock");
	mInputModsStringMap.emplace(InputMod::Ctrl, "Ctrl");
	mInputModsStringMap.emplace(InputMod::NoMod, "NoMod");
	mInputModsStringMap.emplace(InputMod::NumLock, "NumLock");
	mInputModsStringMap.emplace(InputMod::Shift, "Shift");
	mInputModsStringMap.emplace(InputMod::Super, "Super");
}

void UserInput::createKeyCodeMap()
{
	if (mKeyCodeMap.size() == 0)
	{
		for (int i = GLFW_KEY_UNKNOWN + 1; i < GLFW_KEY_LAST; i++)
		{
			const char* ss = glfwGetKeyName(i, 0);
			if (ss)
			{
				std::string s = ss;
				if (s != "")
					mKeyCodeMap[s] = i;
			}
		}
	}
}

UserInput::LogicalOperator UserInput::to_LogicalOperator(const std::string& s)
{
	for (auto& x : mLogicalOperatorStringMap)
	{
		if (x.second == s)
			return x.first;
	}
	return UserInput::LogicalOperator::NoCommand;
}

UserInput::InputMod UserInput::to_InputMod(const std::string& s)
{
	for (auto& x : mInputModsStringMap)
	{
		if (x.second == s)
			return x.first;
	}
	return UserInput::InputMod::NoMod;
}

void UserInput::addKey(int keyCode, int mods, UserInput::LogicalOperator lo)
{
	// First in gets it. This assumes that user preferences is stored first.
	if (mBaseKeyMapping.find({ keyCode, mods }) == mBaseKeyMapping.end())
		mBaseKeyMapping[{keyCode, mods}] = lo;
}

void UserInput::addKeyMapping(const std::string& key, const std::vector<std::string>& mods, 
							const std::string& logicalOperator)
{
	createKeyCodeMap();
	int glfwKey = mKeyCodeMap[key];
	int m = 0;
	for (auto& var : mods)
	{
		UserInput::InputMod im = to_InputMod(var);
		m |= (int)im;
	}
	addKey(glfwKey, m, to_LogicalOperator(logicalOperator));
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
						int key, int scancode, int action, int mods)
{
	UserCommandCallbackData data;
	if (codepoint)
	{
		// If you wish to offer regular text input, set a character callback.
		// https://www.glfw.org/docs/3.3/input_guide.html
		// https://www.glfw.org/docs/3.0/group__input.html
		if (key < 32)
			return;
	}
	else
	{
		if (key < 32)
			return;
		LogStream(LogStreamLevel::Info) << "data.key["
			<< data.key << "] action[" << action << "]\n";
		data.action = InputAction(action);
		data.key = key;
		data.mods = mods;
		data.userCommand = userCommand(data);
		if (data.userCommand == LogicalOperator::NoCommand)
			return;
		for (auto& cb : mUserCommandCallbacks)
		{
			cb.first(data);
		}
	}
}

UserInput::LogicalOperator UserInput::userCommand(const UserCommandCallbackData& data)
{
	auto iter = mBaseKeyMapping.begin();
	while (iter != mBaseKeyMapping.end())
	{
		if (iter->first.first == data.key)
		{
			if (iter->first.second == InputMod::NoMod && data.mods == 0)
				return iter->second;
			if (iter->first.second & data.mods)
				return iter->second;
		}
		++iter;
	}
	return LogicalOperator::NoCommand; 
}

void UserInput::windowResize(void* window, const glm::ivec2& widthHeight)
{
	UserCommandCallbackData data;
	data.action = InputAction::NoAction;
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
	data.action = UserInput::NoAction;
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