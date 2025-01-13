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
	enum LogicalOperator
	{
		NoCommand,
		Save,
		POVForward,
		POVBack,
		POVLeft,
		POVRight,
		POVDown,
		POVUp,
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
		RopeScreen,
		Accept,
		WindowResize,
		WindowClose,
		Special1, Special2, Special3, Special4, Special5, Special6
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
		LogicalOperator userCommand;
	};

	enum PointingDeviceAction
	{
		LeftMouseButtonClick,
		RightMouseButtonClick,
		MiddleMouseButtonClick,
		LeftMouseButtonRelease,
		RightMouseButtonRelease,
		MiddleMouseButtonRelease,
		LeftMouseButtonDrag,
		RightMouseButtonDrag,
		MouseMove,
		Unknown
	};
	enum AnyInputType
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
		PointingDeviceCallbackData()
		: pos({0,0,0}), mods(0), action(PointingDeviceAction::Unknown)
		{ }
		PointingDeviceCallbackData(const glm::vec3& _pos, int _mods, PointingDeviceAction _action)
			:pos(_pos), mods(_mods), action(_action)
		{ }
	};
	struct AnyInput
	{
		AnyInputType inputType;
		PointingDeviceCallbackData mouseInput;
		UserCommandCallbackData keyInput;
	};
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::map<LogicalOperator, std::string> mLogicalOperatorStringMap;
	void createLogicalOperatorStringMap();

	std::map<InputMod, std::string> mInputModsStringMap;
	void createInputModStringMap();

	// https://www.glfw.org/docs/3.3/group__keys.html#gaa06a712e6202661fc03da5bdb7b6e545
	std::map<std::string, int> mKeyCodeMap;
	void createKeyCodeMap();

	LogicalOperator to_LogicalOperator(const std::string& s);
	InputMod to_InputMod(const std::string& s);
	void addMapping(UserInput::LogicalOperator buc, const std::string& s);

/*
	struct BaseKeyMapping
	{
		char key;
		InputMod keyMod;

		friend bool operator<(const BaseKeyMapping& lhs, const BaseKeyMapping& rhs)
		{
			if (lhs.key == rhs.key)
				return lhs.keyMod < rhs.keyMod;
			return lhs.key < rhs.key;
		}
	};

	struct cmpByBaseUserCommand {
		bool operator()(const BaseKeyMapping& lhs, const BaseKeyMapping& rhs) const {
			return lhs.key < rhs.key;
		}
	};
*/

	typedef std::function<void(const UserCommandCallbackData&)> UserCommandCallbackType;
	typedef std::function<void(const PointingDeviceCallbackData&)> PointingDeviceCallbackType;
	typedef std::function<void(void* window, glm::ivec2 dimensions)> WindowResizeCallbackType;

	void init(GLApplication* app);
#pragma warning( pop )
protected:
	void addKey(int keyCode, int mods, UserInput::LogicalOperator lo);
public:
	UserInput();
	virtual void addKeyMapping(const std::string& key, const std::vector<std::string>& mods, 
		const std::string& logicalOperator);
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
	virtual LogicalOperator userCommand(const UserCommandCallbackData& data);
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	glm::ivec2 mFrameBuffer = glm::ivec2(0);
	glm::ivec2 mWindowSize = glm::ivec2(0);
	std::map<std::pair<int, int>, LogicalOperator> mBaseKeyMapping;
	std::vector<std::pair<UserCommandCallbackType, size_t>> mUserCommandCallbacks;
	std::vector<std::pair<PointingDeviceCallbackType, size_t>> mPointingDeviceCallbacks;
	std::vector<std::pair<WindowResizeCallbackType, size_t>> mWindowResizeCallbacks;
	void windowResize(void* window, const glm::ivec2& widthHeight);
	void pointingDevice(void* window, int button, int action, int mods, const glm::vec3& pos);
	void cursorPosition(void* window, const glm::vec3& p);
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