#pragma once
#include <map>
#include <vector>
#include <functional>
#include <iostream>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "ListenerHelper.h"

#define ANY_KEY (GLFW_KEY_LAST+1)

/*
	Converts raw Keyboard and Pointing Device (PD) actions into logical commands.
	Listener callbacks are accessed by subscribers.
*/

class GLApplication;
class OWENGINE_API UserInput
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
		PlaybackStart,
		PlaybackEnd,
		OptionsScreen,
		Accept,
		WindowResize,
		WindowClose
	};
	enum InputAction
	{
		Release = 0,
		Press = 1,
		Repeat = 2,
		NoAction = 3
	};
	enum InputMod
	{
		NoMod = 0x0,
		Shift = 0x0001,
		Ctrl = 0x0002,
		Alt = 0x0004,
		Super = 0x0008,
		CapsLock = 0x00010,
		NumLock = 0x00020
	};
	struct UserCommandCallbackData
	{
		InputAction action;
		int key;
		int mods;
		int userCommand;
	};

	enum PointingDeviceAction
	{
		LeftMouseButtonClick,
		RightMouseButtonClick,
		MiddleMouseButtonClick,
		LeftMouseButtonDrag,
		RightMouseButtonDrag,
		MouseMove,
		Unknown
	};
	enum class AnyInputType
	{
		Pointing,
		KeyPress,
		CodePoint
	};
	struct PointingDeviceCallbackData
	{
		glm::vec3 pos;
		int mods;
		PointingDeviceAction action;
	};
	struct AnyInput
	{
		AnyInputType inputType;
		PointingDeviceCallbackData mouseInput;
		UserCommandCallbackData keyInput;
	};
	static BaseUserCommand to_BaseUserCommand(const std::string& s);
	static InputMod to_InputMod(const std::string& s);
private:
	static std::map<BaseUserCommand, std::string> mBaseUserCommandMap;
	void createBaseUserCommandMap();

	static std::map<InputMod, std::string> mInputModsMap;
	void createInputModMap();

	struct BaseKeyMapping
	{
		int userCommand;
		InputMod keyMod;

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

	typedef std::function<void(const UserCommandCallbackData&)> UserCommandCallbackType;
	typedef std::function<void(const PointingDeviceCallbackData&)> PointingDeviceCallbackType;
	typedef std::function<void(void* window, glm::ivec2 dimensions)> WindowResizeCallbackType;

	void init(GLApplication* app);
public:
	UserInput();
	void addUserCommandListener(UserCommandCallbackType cb,
		const ListenerHelper* helper = nullptr)
	{ 
		mUserCommandCallbacks.push_back({ cb, helper ? helper->mUniqueId : 0 });
	}
	void removeUserCommandListener(const ListenerHelper* helper);

	void addPointingDeviceListener(PointingDeviceCallbackType cb,
		const ListenerHelper* helper = nullptr)
	{
		mPointingDeviceCallbacks.push_back({ cb, helper ? helper->mUniqueId : 0 });
	}
	void removePointingDeviceListener(const ListenerHelper* helper);

	void addWindowResizeListener(WindowResizeCallbackType cb,
		const ListenerHelper* helper = nullptr)
	{
		mWindowResizeCallbacks.push_back({ cb, helper ? helper->mUniqueId : 0 });
	}

	void removeWindowResizeListener(const ListenerHelper* helper);


protected:
	virtual int userCommand(const UserCommandCallbackData& data);
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::ivec2 mFrameBuffer = glm::ivec2(0);
	glm::ivec2 mWindowSize = glm::ivec2(0);
	std::map<BaseKeyMapping, BaseUserCommand> mBaseKeyMapping;
	std::vector< std::pair<UserCommandCallbackType, size_t>> mUserCommandCallbacks;
	std::vector<std::pair<PointingDeviceCallbackType, size_t>> mPointingDeviceCallbacks;
	std::vector<std::pair<WindowResizeCallbackType, size_t>> mWindowResizeCallbacks;
	void windowResize(void* window, const glm::ivec2& widthHeight);
	void pointingDevice(void* window, int button, int action, int mods, const glm::vec3& pos);
	void cursorPosition(void* window, double x, double y);
	void keyboard(unsigned int codepoint, int key, int scancode, int action, int mods);
	void closeWindow(void* window);

	virtual std::string userInputToString(int value) = 0;
#pragma warning( pop )
	friend class GLApplication;
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