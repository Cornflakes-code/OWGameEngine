#pragma once
#include <map>
#include <vector>
#include <functional>
#include <iostream>

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ANY_KEY (GLFW_KEY_LAST+1)

/*
	Converts raw Keyboard and Pointing Device (PD) actions into logical commands.
	Listener callbacks can be added by subscribers.
*/

class GLApplication;
class UserInput
{
public:
	static int AnyKey;
	enum BaseUserCommand
	{
		NoCommand,
		Save,
		POVForward,
		POVBack,
		POVLeft,
		POVRight,
		POVPitchDown,
		POVPitchUp,
		POVYawLeft,
		POVYawRight,
		POVRollClockwise,
		POVRollAntiClockwise,
		Restore,
		RecordingStart,
		RecordingEnd,
		OptionsScreen,
		Accept,
		NumBaseCommands
	};

	enum InputMods
	{
		NoMod = 0x0,
		Shift = 0x0001,
		Ctrl = 0x0002,
		Alt = 0x0004,
		Super = 0x0008,
		CapsLock = 0x00010,
		NumLock = 0x00020
	};

	struct BaseKeyMapping
	{
		int userCommand;
		InputMods keyMod;

		friend bool operator<(const BaseKeyMapping& lhs, const BaseKeyMapping& rhs)
		{
			if (lhs.userCommand == rhs.userCommand)
				return lhs.keyMod < rhs.keyMod;
			return lhs.userCommand < rhs.userCommand;
		}
	};

	struct cmpByBaseUserCommand {
		bool operator()(const BaseKeyMapping& lhs, const BaseKeyMapping& rhs) const {
			return lhs.userCommand < rhs.userCommand;
		}
	};
	struct UserCommandCallbackData
	{
		int action;
		int key;
		int mods;
		int userCommand;
	};

	enum class PointingDeviceAction
	{
		LeftMouseButtonClick,
		RightMouseButtonClick,
		MiddleMouseButtonClick,
		LeftMouseButtonDrag,
		RightMouseButtonDrag,
		MouseMove,
		Unknown
	};
	struct PointingDeviceCallbackData
	{
		glm::vec3 pos;
		int mods;
		PointingDeviceAction action;
	};
	enum class AnyInputType
	{
		Pointing,
		KeyPress,
		CodePoint
	};
	struct AnyInput
	{
		AnyInputType inputType;
		PointingDeviceCallbackData mouseInput;
		UserCommandCallbackData keyInput;
	};

	typedef std::function<void(const UserCommandCallbackData&)> UserCommandCallbackType;
	typedef std::function<void(const PointingDeviceCallbackData&)> PointingDeviceCallbackType;

	UserInput();

	void init(GLApplication* app);
	void addUserCommandListener(UserCommandCallbackType callback)
		{ mUserCommandCallbacks.push_back(callback); }

	void addPointingDeviceListener(PointingDeviceCallbackType callback)
		{ mPointingDeviceCallbacks.push_back(callback); }

protected:
	virtual int userCommand(const UserCommandCallbackData& data);
private:
	glm::ivec2 mFrameBuffer = glm::ivec2(0);
	glm::ivec2 mWindowSize = glm::ivec2(0);
	void doMouseClick(GLFWwindow* window, int button, int action, int mods);
	void doKeyPressCallback(unsigned int codepoint, int key, int scancode, int action, int mods);
	std::map<BaseKeyMapping, BaseUserCommand> mBaseKeyMapping;

	std::vector<UserCommandCallbackType> mUserCommandCallbacks;
	std::vector<PointingDeviceCallbackType> mPointingDeviceCallbacks;
	virtual std::string userInputToString(int value) = 0;
};

/*
typedef void(*GLFWmousebuttonfun)(GLFWwindow*, int, int, int);
GLFWAPI GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun callback);

typedef void(*GLFWwindowcontentscalefun)(GLFWwindow*, float, float);
GLFWAPI GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback(GLFWwindow* window, GLFWwindowcontentscalefun callback);

typedef void(*GLFWcursorposfun)(GLFWwindow*, double, double);
GLFWAPI GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);

typedef void(*GLFWcursorenterfun)(GLFWwindow*, int);
GLFWAPI GLFWcursorenterfun glfwSetCursorEnterCallback(GLFWwindow* window, GLFWcursorenterfun callback);

typedef void(*GLFWscrollfun)(GLFWwindow*, double, double);
GLFWAPI GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback);

typedef void(*GLFWkeyfun)(GLFWwindow*, int, int, int, int);
GLFWAPI GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback);

typedef void(*GLFWcharfun)(GLFWwindow*, unsigned int);
GLFWAPI GLFWcharfun glfwSetCharCallback(GLFWwindow* window, GLFWcharfun callback);

typedef void(*GLFWcharmodsfun)(GLFWwindow*, unsigned int, int);
GLFWAPI GLFWcharmodsfun glfwSetCharModsCallback(GLFWwindow* window, GLFWcharmodsfun callback);

typedef void(*GLFWjoystickfun)(int, int);
GLFWAPI GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun callback);
*/