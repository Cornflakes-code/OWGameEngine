#pragma once
#include <functional>
#include <iostream>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Movie.h"
#include "ListenerHelper.h"
/*
	Provides access to GLFW callback functionality.
*/
struct GLFWwindow;
class Movie;

class Logger;
class ResourceFactory;
class MacroRecorder;
class SaveAndRestore;
class UserInput;

class GLApplication
{
public:
	void init(Movie* movie, UserInput* ui, 
			  MacroRecorder* recorder, SaveAndRestore* saveRestore);
	void run(Movie* movieSaveAndRestore);
	static GLApplication* getApplication(ResourceFactory* mr, UserInput* ui);

	// convenience methods
	glm::vec2 pointingDevicePosition() const { return mPointingDevicePosition; }
	static float secondsSinceLoad();
	const glm::uvec2& physicalWindowSize() const { return mPhysicalWindowSize; }
	void physicalWindowSize(const glm::uvec2& newValue) { mPhysicalWindowSize = newValue; }
	SaveAndRestore* saveAndRestore() { return mSaveAndRestore;  }
	const Movie* movie() const { return mMovie; }
	enum class WindowResizeType
	{
		FrameBuffer, WindowResize
	};
	// callbacks
	typedef std::function<void(GLFWwindow* window)> WindowCloseCallbackType;
	typedef std::function<void(WindowResizeType resizeType, glm::ivec2 dimensions)> WindowResizeCallbackType;
	typedef std::function<void(GLFWwindow* window, int button, int action, int mods)> PointingDeviceCallbackType;
	typedef std::function<void(unsigned int codepoint, 
			int key, int scancode, int action, int mods)> KeyboardCallbackType;
	typedef std::function<void(GLFWwindow*, double, double)> CursorPositionCallback;

	void errorReporting(int error, const char* description);
	void addWindowCloseListener(WindowCloseCallbackType cb)
	{
		mWindowCloseListeners.push_back(cb);
	}
	void addKeyboardListener(KeyboardCallbackType cb)
			{ mKeyboardCallbacks.push_back(cb); }

	void addPointingDeviceListener(PointingDeviceCallbackType cb)
			{ mPointingDeviceCallbacks.push_back(cb); }

	void addCursorPositionCallback(CursorPositionCallback cb,
		const ListenerHelper* helper = nullptr)
	{
		mCursorPositionCallbacks.push_back({ cb, helper ? helper->mUniqueId : 0 });
	}	
	void removeCursorPositionCallback(const ListenerHelper* helper);
	void addWindowResizeListener(WindowResizeCallbackType cb, 
								 const ListenerHelper* helper = nullptr)
	{
		mWindowResizeCallbacks.push_back({ cb, helper? helper->mUniqueId : 0 });
	}
	void removeWindowResizeListener(const ListenerHelper* helper);
private:
	GLApplication(ResourceFactory* _factory, UserInput* ui);
	~GLApplication();

	SaveAndRestore* mSaveAndRestore = nullptr;
	glm::uvec2 mPhysicalWindowSize;
	GLFWwindow* mWindow;
	Logger* mLogger;
	glm::vec2 mPointingDevicePosition;
	static OWUtils::Time::time_point mLoadTime;
	const Movie* mMovie = nullptr;
	// mUserInput needed because glfwSetWindowUserPointer pointing to one thing only;
	UserInput* mUserInput;
	std::vector<WindowCloseCallbackType> mWindowCloseListeners;
	std::vector<KeyboardCallbackType> mKeyboardCallbacks;
	std::vector<PointingDeviceCallbackType> mPointingDeviceCallbacks;
	std::vector<std::pair<WindowResizeCallbackType, size_t>> mWindowResizeCallbacks;
	std::vector<std::pair<CursorPositionCallback, size_t>> mCursorPositionCallbacks;

	void enableCallbacks(); 

	// Keyboard callbacks
	virtual void onCharCallback(GLFWwindow* window, unsigned int codepoint);
	virtual void onKeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// Pointing Device Callbacks
	virtual void onPointingDeviceCallback(GLFWwindow* window, int button, int action, int mods);
	virtual void onPointingDevicePositionCallback(GLFWwindow* window, double x, double y);
	// Window resize callbacks
	virtual void onFrameBufferResizeCallback(GLFWwindow* window, int width, int height);
	virtual void onSetWindowSizeCallback(GLFWwindow* window, int width, int height);

	// Other Callbacks
	virtual void onCloseCallback(GLFWwindow* window);
};

/*
typedef void(*GLFWerrorfun)(int, const char*);
GLFWAPI GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun callback);

typedef void(*GLFWwindowposfun)(GLFWwindow*, int, int);
GLFWAPI GLFWwindowposfun glfwSetWindowPosCallback(GLFWwindow* window, GLFWwindowposfun callback);

typedef void(*GLFWwindowsizefun)(GLFWwindow*, int, int);
GLFWAPI GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun callback);

typedef void(*GLFWwindowclosefun)(GLFWwindow*);
GLFWAPI GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun callback);

typedef void(*GLFWwindowrefreshfun)(GLFWwindow*);
GLFWAPI GLFWwindowrefreshfun glfwSetWindowRefreshCallback(GLFWwindow* window, GLFWwindowrefreshfun callback);

typedef void(*GLFWwindowfocusfun)(GLFWwindow*, int);
GLFWAPI GLFWwindowfocusfun glfwSetWindowFocusCallback(GLFWwindow* window, GLFWwindowfocusfun callback);

typedef void(*GLFWwindowiconifyfun)(GLFWwindow*, int);
GLFWAPI GLFWwindowiconifyfun glfwSetWindowIconifyCallback(GLFWwindow* window, GLFWwindowiconifyfun callback);

typedef void(*GLFWwindowmaximizefun)(GLFWwindow*, int);
GLFWAPI GLFWwindowmaximizefun glfwSetWindowMaximizeCallback(GLFWwindow* window, GLFWwindowmaximizefun callback);

typedef void(*GLFWdropfun)(GLFWwindow*, int, const char*[]);
GLFWAPI GLFWdropfun glfwSetDropCallback(GLFWwindow* window, GLFWdropfun callback);

typedef void(*GLFWmonitorfun)(GLFWmonitor*, int);
GLFWAPI GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun callback);

*/