#include "GLApplication.h"

// Every app needs some globals. No need to get fancy. 
// Bite the bullet and call them what they are.
OWENGINE_API GlobalSettings* globals = nullptr;

#include <algorithm>

#include "../Helpers/ErrorHandling.h"
#include "../Helpers/Logger.h"
#include "SaveAndRestore.h"

#pragma comment( lib, "glfw3.lib" )
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "freetype.lib")

static GLApplication* hackForErrorCallback = nullptr;
void error_callback(int error, const char* description);

GLApplication::GLApplication(UserInput* ui)
	:mUserInput(ui)
{
	globals = new GlobalSettings();
	globals->application(this);
}

GLApplication::~GLApplication()
{
	if (hackForErrorCallback == this)
		hackForErrorCallback = nullptr;
}

void GLApplication::init(Movie* movie, UserInput* ui, 
						 MacroRecorder* recorder, SaveAndRestore* saveRestore)
{
	globals->mPhysicalWindowSize = saveRestore->physicalWindowSize();
	globals->movie(movie);
	globals->saveAndRestore(saveRestore);
	globals->recorder(recorder);
	if (glfwInit())
	{
		// https://antongerdelan.net/opengl/glcontext2.html

		// You'll find a list of all the key codes and other input handling commands 
		// at http://www.glfw.org/docs/latest/group__input.html. 
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_SAMPLES, 4); // anti- aliasing. 16 for really high

#ifdef SECOND_MONITOR_WINDOW
		int count;
		GLFWmonitor** mons = glfwGetMonitors(&count);
		GLFWmonitor* mon = mons[1];
		const GLFWvidmode* mode = glfwGetVideoMode(mon);

		mWindow = glfwCreateWindow(mScrWidth, mScrHeight, windowTitle, mon, NULL);
		glfwSetWindowMonitor(window, NULL, -1000, 200, mScrWidth, mScrHeight, mode->refreshRate);
#else
		mWindow = glfwCreateWindow(globals->mPhysicalWindowSize.x,
			globals->mPhysicalWindowSize.y,
						movie->windowTitle().c_str(), NULL, NULL);
		if (!mWindow)
		{
			throw NMSException("Could not create OpenGL window.");
		}
		glfwSetWindowMonitor(mWindow, NULL, 2000, 200, 
			globals->mPhysicalWindowSize.x,
			globals->mPhysicalWindowSize.y, GLFW_DONT_CARE);
#endif
		glfwMakeContextCurrent(mWindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw NMSException("gladLoadGLLoader failed.");
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		checkGLError();

		// tell GL to only draw onto a pixel if the shape is closer to the viewer
		glEnable(GL_DEPTH_TEST); // enable depth-testing
		glDepthFunc(GL_LEQUAL); // depth-testing interprets a smaller value as "closer"

		// mFrameBufferSize and mWindowSize may differ. See:
		// (https://stackoverflow.com/questions/45796287/screen-coordinates-to-world-coordinates)

		globals->mLogger->log_gl_params(std::cout);
		enableCallbacks();
		try
		{
			movie->init(this, ui, recorder);
			mUserInput->init(this);
			// We set the window size before the callbacks were hooked up.
			onFrameBufferResizeCallback(mWindow, 
				globals->mPhysicalWindowSize.x,
				globals->mPhysicalWindowSize.y);
			onSetWindowSizeCallback(mWindow, 
				globals->mPhysicalWindowSize.x,
				globals->mPhysicalWindowSize.y);
		}

		catch (const std::exception& ex)
		{
			std::cout << ex.what() << " press any key\n";
			int ch;
			std::cin >> ch;
			exit(-1);
		}
	}
	else
	{
		throw NMSException("glfwInit() failed");
	}
}

void GLApplication::run(Movie* movie)
{
	//display.print_all(shader.program(), std::cout);
	movie->preRun();
	movie->run(mUserInput, mWindow);
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void GLApplication::removeWindowResizeListener(const ListenerHelper* helper)
{
	if (!helper)
	{
		return;
	}
	auto iter = mWindowResizeCallbacks.begin();
	while (iter != mWindowResizeCallbacks.end())
	{
		if (iter->second == helper->mUniqueId)
		{
			mWindowResizeCallbacks.erase(iter);
			return;
		}
		++iter;
	}
}

void GLApplication::removeCursorPositionCallback(const ListenerHelper* helper)
{
	if (!helper)
	{
		return;
	}
	auto iter = mCursorPositionCallbacks.begin();
	while (iter != mCursorPositionCallbacks.end())
	{
		if (iter->second == helper->mUniqueId)
		{
			mCursorPositionCallbacks.erase(iter);
			return;
		}
		++iter;
	}
}

//void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
//	GLsizei length, const char *message, const void *userParam)
//{}

void GLApplication::enableCallbacks()
{
	hackForErrorCallback = this;
	glfwSetWindowUserPointer(mWindow, this);

	glfwSetErrorCallback([](int error, const char* description) {
		error_callback(error, description);
	});

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
	{
		// https://learnopengl.com/In-Practice/Debugging
		// initialize debug output 
		//glEnable(GL_DEBUG_OUTPUT);
		//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//glDebugMessageCallback(glDebugOutput, nullptr);
		//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
		auto pointer = reinterpret_cast<GLApplication*>(glfwGetWindowUserPointer(window));
		pointer->onCloseCallback(window);
	});

	glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
		auto pointer = reinterpret_cast<GLApplication*>(glfwGetWindowUserPointer(window));
		pointer->onFrameBufferResizeCallback(window, width, height);
	});

	glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
		auto pointer = reinterpret_cast<GLApplication*>(glfwGetWindowUserPointer(window));
		pointer->onSetWindowSizeCallback(window, width, height);
	});

	glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto pointer = reinterpret_cast<GLApplication*>(glfwGetWindowUserPointer(window));
		pointer->onKeyPressCallback(window, key, scancode, action, mods);
	});

	glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int scancode) {
		auto pointer = reinterpret_cast<GLApplication*>(glfwGetWindowUserPointer(window));
		pointer->onCharCallback(window, scancode);
	});

	glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods) {
		auto pointer = reinterpret_cast<GLApplication*>(glfwGetWindowUserPointer(window));
		pointer->onPointingDeviceCallback(window, button, action, mods);
	});

	glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double x, double y) {
		auto pointer = reinterpret_cast<GLApplication*>(glfwGetWindowUserPointer(window));
		pointer->onPointingDevicePositionCallback(window, x, y);
	});

}

void GLApplication::onFrameBufferResizeCallback(GLFWwindow* OW_UNUSED(window), 
								int width, int height)
{
	glViewport(0, 0, width, height);

	for (auto& cb : mWindowResizeCallbacks)
		cb.first(WindowResizeType::FrameBuffer, glm::ivec2(width, height));
}

void GLApplication::onSetWindowSizeCallback(GLFWwindow* OW_UNUSED(window), 
								int width, int height)
{
	for (auto& cb : mWindowResizeCallbacks)
		cb.first(WindowResizeType::WindowResize, glm::vec2(width, height));
}

void GLApplication::onPointingDeviceCallback(GLFWwindow* window, 
										int button, int action, int mods)
{
	for (auto& cb : mPointingDeviceCallbacks)
		cb(window, button, action, mods);
}

void GLApplication::onPointingDevicePositionCallback(GLFWwindow* window, double x, double y)
{
	globals->mPointingDevicePosition.x = static_cast<glm::vec2::value_type>(x);
	globals->mPointingDevicePosition.y = static_cast<glm::vec2::value_type>(y);

	for (auto& cb : mCursorPositionCallbacks)
		cb.first(window, x, y);
}

void GLApplication::onCharCallback(GLFWwindow* OW_UNUSED(window), unsigned int codepoint)
{
	for (auto& cb : mKeyboardCallbacks)
		cb(codepoint, 0, 0, 0, 0);
}

void GLApplication::onKeyPressCallback(GLFWwindow* OW_UNUSED(window), 
								int key, int scancode, int action, int mods)
{
	for (auto& cb : mKeyboardCallbacks)
		cb(0, key, scancode, action, mods);
}

void GLApplication::onCloseCallback(GLFWwindow* window)
{
	for (auto& cb : mWindowCloseListeners)
		cb(window);	
}

void error_callback(int error, const char* description)
{
	// https://github.com/Polytonic/Glitter
	if (hackForErrorCallback)
		hackForErrorCallback->errorReporting(error, description);
	else
		std::cout << "GLFW ERROR: code [" << error << "] msg [" << description << "]\n";
}

void GLApplication::errorReporting(int error, const char* description)
{
	std::cout << "GLFW ERROR: code [" << error << "] msg [" << description << "]\n";
}

/*
Unused Callbacks

GLFWAPI GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun callback);
GLFWAPI GLFWwindowposfun glfwSetWindowPosCallback(GLFWwindow* window, GLFWwindowposfun callback);
GLFWAPI GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun callback);

GLFWAPI GLFWwindowrefreshfun glfwSetWindowRefreshCallback(GLFWwindow* window, GLFWwindowrefreshfun callback);
GLFWAPI GLFWwindowfocusfun glfwSetWindowFocusCallback(GLFWwindow* window, GLFWwindowfocusfun callback);
GLFWAPI GLFWwindowiconifyfun glfwSetWindowIconifyCallback(GLFWwindow* window, GLFWwindowiconifyfun callback);
GLFWAPI GLFWwindowmaximizefun glfwSetWindowMaximizeCallback(GLFWwindow* window, GLFWwindowmaximizefun callback);

GLFWAPI GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);
GLFWAPI GLFWcursorenterfun glfwSetCursorEnterCallback(GLFWwindow* window, GLFWcursorenterfun callback);
GLFWAPI GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback);


GLFWAPI GLFWdropfun glfwSetDropCallback(GLFWwindow* window, GLFWdropfun callback);
GLFWAPI GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun callback);
*/
