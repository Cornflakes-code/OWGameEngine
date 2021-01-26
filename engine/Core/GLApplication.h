#pragma once
#include <functional>
#include <iostream>
#include <experimental/filesystem>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>

#include "../OWEngine/OWEngine.h"

#include "ListenerHelper.h"

/*
	Provides access to GLFW callback functionality.
*/
struct GLFWwindow;
class Movie;

class Logger;
class MacroRecorder;
class SaveAndRestore;
class UserInput;
class GlobalSettings;
class Camera;

class OWENGINE_API GLApplication
{
public:
	GLApplication(UserInput* ui);
	~GLApplication();

	void init(Movie* movie, UserInput* ui, MacroRecorder* recorder, 
			SaveAndRestore* saveRestore, Camera* camera);
	void run(Movie* movieSaveAndRestore);
	void errorReporting(int error, const char* description);
	void onDebugMessageCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar *message, const void *userParam);
	glm::vec4 backgroundColour(const glm::vec4& newValue, unsigned int clearMask = UINT_MAX);
	void restoreBackgroundColour();
	void clearBuffers()
	{
		glClear(mBackgroundColourMask);
	}

private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	GLFWwindow* mWindow = nullptr;
	glm::ivec2 mFrameBuffer = glm::ivec2(0);
	glm::ivec2 mWindowSize = glm::ivec2(0);
	glm::vec4 mBackgroundColour;
	glm::vec4 mPriorBackgroundColour;
	unsigned int mBackgroundColourMask;
	// GLApplication knows nothing about logical commands. UserInput translates physical 
	// commands to logical commands. UserInput could be subclassed to handle different
	// types of physical systems.
	UserInput* mUserInput;

	void enableCallbacks(); 

	// Keyboard callbacks
	virtual void onCharCallback(GLFWwindow* window, unsigned int codepoint);
	virtual void onKeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// Pointing Device Callbacks
	virtual void onPointingDeviceCallback(GLFWwindow* window, int button, int action, int mods);
	virtual void onPointingDevicePositionCallback(GLFWwindow* window, double x, double y);
	// Window resize callbacks
	virtual void onFrameBufferResizeCallback(GLFWwindow* window, int width, int height);

	// Other Callbacks
	virtual void onCloseCallback(GLFWwindow* window);
#pragma warning( pop )
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
