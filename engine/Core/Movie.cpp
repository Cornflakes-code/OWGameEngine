#include "Movie.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Helpers/ErrorHandling.h"
#include "../Helpers/Logger.h"
#include "Camera.h"

#include "GLApplication.h"
#include "QuitScene.h"
#include "Scene.h"

Movie::Movie(const std::string& _windowTitle, 
			 const glm::uvec2& _windowSize, 
			 Camera* _camera)
: mLogger(new Logger())
, mWindowTitle(_windowTitle)
, mPhysicalWindowSize(_windowSize)
, mCamera(_camera)
{
}

void Movie::preRun()
{
	Scene* s = new QuitScene(this);
	this->add(s, new QuitScenePhysics(s), false);
}

void Movie::init(GLApplication* app, UserInput* ui, MacroRecorder* recorder)
{
	app->addWindowResizeListener([this](GLApplication::WindowResizeType resizeType, glm::ivec2 newSize)
	{
		// There is a lot more to do this correctly
		// https://stackoverflow.com/questions/45796287/screen-coordinates-to-world-coordinates

		if (resizeType == GLApplication::WindowResizeType::FrameBuffer)
		{
			glViewport(0, 0, newSize.x, newSize.y);
			mPhysicalWindowSize = newSize;
			mCamera->aspectRatio(newSize.y / (1.0f*newSize.x));
		}
		else if (resizeType == GLApplication::WindowResizeType::WindowResize)
		{
			// Do not pass the window size to glViewport or other pixel-based OpenGL calls.
			// The window size is in screen coordinates, not pixels.Use the framebuffer 
			// size, which is in pixels, for pixel - based calls.
			// https://www.glfw.org/docs/latest/window_guide.html#window_size
		}
	});
	ui->addUserCommandListener([this](const UserInput::UserCommandCallbackData& data)
	{
		UserInput::AnyInput ai;
		if (false)//data.codepoint)
		{
			// This is for user typing stuff. Have not handled this yet
			ai.inputType = UserInput::AnyInputType::CodePoint;
		}
		else
		{
			ai.inputType = UserInput::AnyInputType::KeyPress;
			ai.keyInput = data;
			pushUserInput(ai);
		}
	});
	ui->addPointingDeviceListener([this](const UserInput::PointingDeviceCallbackData& data)
	{
		UserInput::AnyInput ai;
		ai.inputType = UserInput::AnyInputType::Pointing;
		ai.mouseInput = data;
		pushUserInput(ai);
	});
	mLogger->log_gl_params(std::cout);
}

void Movie::run(UserInput* ui)
{
	if (!mScenes.size())
	{
		throw NMSLogicException(
			"Error. Calling Movie::run() without no Scenes is not a good thing.\n");
	}
	if (!mCurrent)
	{
		throw NMSLogicException(
			"Error. Calling Movie::run() with no current Scene is not a good thing.\n");
	}

	// https://gamedev.stackexchange.com/questions/1589/when-should-i-use-a-fixed-or-variable-time-step
	// https://gafferongames.com/post/integration_basics/
	// https://gafferongames.com/post/fix_your_timestep/ // The Gold Standard??
	// https://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
	// https://stackoverflow.com/questions/20390028/c-using-glfwgettime-for-a-fixed-time-step
	// https://gamedev.stackexchange.com/questions/651/how-should-i-write-a-main-game-loop

	// https://www.pluralsight.com/blog/software-development/how-to-measure-execution-time-intervals-in-c--
	// Also see below for fix_your_time_step integration
	// https://github.com/TylerGlaiel/FrameTimingControl/blob/master/frame_timer.cpp

	// Multi threaded render loop
	// http://blog.slapware.eu/game-engine/programming/multithreaded-renderloop-part1/
	int hz = 1000 / 60;

	// Need to code some feedback to ensure that logic.fixedUpdate does not take longer than dt.
	// Or is this already handled by the loop?
	const OWUtils::Time::duration dt = std::chrono::milliseconds(hz/2);
	const OWUtils::Time::duration clamp = dt * 8;

	OWUtils::Time::duration t = std::chrono::seconds(0);
	OWUtils::Time::time_point currentTime = OWUtils::Time::now();
	OWUtils::Time::duration accumulator = std::chrono::seconds(0);

	//processTimeStep(lcs, currentScene()->logic()->current, t, std::chrono::milliseconds(0));
	std::string nextSceneName;
	while (!glfwWindowShouldClose(mWindow))
	{
		mLogger->update_fps_counter(mWindow);

		OWUtils::Time::time_point newTime = OWUtils::Time::now();
		OWUtils::Time::duration frameTime = newTime - currentTime;
		currentTime = newTime;
		if (frameTime > clamp)
			frameTime = clamp;

		accumulator += frameTime;

		glClearColor(0.5, 0.5, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		while (accumulator >= dt)
		{
			mCurrent->logic.copyCurrentToPrevious();
			t += dt;
			//dumpMessage("processTimeStep", NMSErrorLevel::NMSCritical);
			processUserInput(nextSceneName, dt); 
			if (nextSceneName.empty())
			{
				mCurrent->logic.fixedUpdate(nextSceneName, dt);
			}
			else
			{
				mCurrent->scene->cumulativeTime(t);
				makeCurrent(nextSceneName);
				t = mCurrent->scene->cumulativeTime();
				nextSceneName = "";
				dumpMessage("processTimeStep++", NMSErrorLevel::NMSCritical);
				mCurrent->logic.fixedUpdate(nextSceneName, dt);
			}
			accumulator -= dt;
		}

		// logic()->interpolateRenderTarget will never change currentScene
		//dumpMessage("render", NMSErrorLevel::NMSCritical);
		mCurrent->logic.interpolateRenderTarget(t, accumulator, dt);

		//OWUtils::Time::time_point beforeRender = OWUtils::HighTime::now();
		render(mCurrent->logic.renderTarget());
		//OWUtils::Time::time_point afterRender = OWUtils::HighTime::now();
		//OWUtils::Time::duration durr = afterRender - beforeRender;
		//dumpMessage(std::stringstream() << "Render Time [" << durr.count() << "]", NMSErrorLevel::NMSInfo);

		mCurrent->logic.clear();
		// https://discourse.glfw.org/t/newbie-questions-trying-to-understand-glfwswapinterval/1287
		// Best practice is to default glfwSwapInterval to '1'
			// 0: do not wait for vsync(may be overridden by driver / driver settings)
			// 1 : wait for 1st vsync(may be overridden by driver / driver settings)
		glfwSwapInterval(1);
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

void Movie::makeCurrent(const std::string& newSceneName)
{
	std::string safeSceneName = newSceneName;
	if (newSceneName == Scene::previousScene())
	{
		if (mPrevious)
		{
			safeSceneName = mPrevious->scene->name();
		}
		else
		{
			safeSceneName = Scene::quitSceneName();
		}
	}
	auto it = mScenes.find(safeSceneName);
	if (it == mScenes.end())
	{
		glfwSetWindowShouldClose(mWindow, true);
		safeSceneName = Scene::quitSceneName();
		dumpMessage(std::stringstream()
			<< "Unknown Scene Name [" << newSceneName
			<< "] passed to makeCurrent()\n", NMSErrorLevel::NMSCritical);
	}
	makeCurrent(&(mScenes[safeSceneName]));

}

void Movie::makeCurrent(LoopControlStruct* lcs)
{
	mPrevious = mCurrent;
	mCurrent = lcs;
	if (!mCurrent->setupCalled)
	{
		mCurrent->scene->setup(mCurrent->logic.current);
		mCurrent->setupCalled = true;
	}
	mCurrent->logic.copyCurrentToPrevious();
	mCurrent->scene->activate(mPrevious ? mPrevious->scene->name() : "", 
			mCurrent->logic.current,
			mCamera, mCurrent->countActivateCalled++);
}

void Movie::render(const ScenePhysicsState* state)
{
	// https://en.wikibooks.org/wiki/OpenGL_Programming
	mCamera->update();
	glm::mat4 projection = mCamera->projection();
	glm::mat4 view = mCamera->view();
	mCurrent->scene->render(state, projection, view);
}

void Movie::add(Scene* toAdd, ScenePhysicsState* sps, bool makeThisSceneCurrent)
{
	if (mScenes.find(toAdd->name()) != mScenes.end())
	{
		throw NMSException(std::stringstream() << "Error. Calling Movie::add for ["
			<< toAdd->name() << "] a second time. This may be bad.\n");

	}
	else
	{
		LoopControlStruct lcs;
		lcs.logic.current = sps;
		lcs.logic.previous = sps->clone();
		lcs.logic.tempRenderTarget = sps->clone();
		lcs.scene = toAdd;
		mScenes[toAdd->name()] = lcs;
		if (makeThisSceneCurrent)
			makeCurrent(&mScenes[toAdd->name()]);
	}
}

void Movie::pushUserInput(const UserInput::AnyInput& userInput)
{
	if (mUserInput.empty())
	{
		mUserInput.push(userInput);
	}
	else
	{
		UserInput::AnyInput& front = mUserInput.front();
		mUserInput.push(userInput);
	}
}

void Movie::processUserInput(std::string& nextScene, OWUtils::Time::duration dt)
{
	OWUtils::Float timeStep = std::chrono::duration<float>(dt).count();

	while (!mUserInput.empty())
	{
		UserInput::AnyInput& input = mUserInput.front();
		dumpMessage(std::stringstream() << "User Input [" 
				<< input.keyInput.userCommand
				<< "]\n", NMSErrorLevel::NMSInfo);
		if (!mCurrent->logic.current->processUserCommands(input, nextScene, 
				this->mPhysicalWindowSize, mCamera))
		{
			mCamera->processInput(input, timeStep);
		}
		if (!nextScene.empty())
		{
			std::queue<UserInput::AnyInput> empty;
			mUserInput.swap(empty);
			return;
		}
		mUserInput.pop();
	}
}

void Movie::physicalWindowSize(const glm::uvec2& newSize)
{

}
