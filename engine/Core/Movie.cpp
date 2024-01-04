#include "Movie.h"

#include <iostream>
#include <thread>

#include <GLFW/glfw3.h>

#include "ErrorHandling.h"
#include "Logger.h"
#include "LogStream.h"

#include "Camera.h"
#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "GLApplication.h"
#include "GlobalSettings.h"
#include "QuitScene.h"
#include "Scene.h"

Movie::Movie(const std::string& _windowTitle, 
			 Camera* _camera, Logger* logger)
: mLogger(logger)
, mCamera(_camera)
, mWindowTitle(_windowTitle)
{
}

void Movie::preRun()
{
	Scene* s = new QuitScene(this);
	this->add(s, new QuitScenePhysics(s), false);
}

void Movie::init(GLApplication* OW_UNUSED(app), UserInput* ui, 
							MacroRecorder* OW_UNUSED(recorder))
{
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
	mLogger->log_gl_params();
}

void Movie::run(UserInput* OW_UNUSED(ui), GLFWwindow* glfwWindow)
{
	if (!mScenes.size())
	{
		throw NMSLogicException(
			"Error. Calling Movie::run() with no Scenes is not a good thing.\n");
	}
	if (!mCurrent)
	{
		throw NMSLogicException(
			"Error. Calling Movie::run() without a current Scene is not a good thing.\n");
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

	while (!glfwWindowShouldClose(glfwWindow))
	{
		mLogger->update_fps_counter(glfwWindow);

		OWUtils::Time::time_point newTime = OWUtils::Time::now();
		OWUtils::Time::duration frameTime = newTime - currentTime;
		currentTime = newTime;
		if (frameTime > clamp)
			frameTime = clamp;
		accumulator += frameTime;
		globals->application()->clearBuffers();
		while (accumulator >= dt)
		{
			mCurrent->logic.copyCurrentToPrevious();
			t += dt;
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
				LogStream(LogStreamLevel::ImportantInfo) << "processTimeStep++";
				mCurrent->logic.fixedUpdate(nextSceneName, dt);
			}
			accumulator -= dt;
		}

		// logic()->interpolateRenderTarget will never change currentScene
		mCurrent->logic.interpolateRenderTarget(t, accumulator, dt);
		//OWUtils::Time::time_point beforeRender = OWUtils::HighTime::now();
		render(mCurrent->logic.renderTarget());
		globals->clearAspectRatioChangedFlag();
		//OWUtils::Time::time_point afterRender = OWUtils::HighTime::now();
		//OWUtils::Time::duration durr = afterRender - beforeRender;
		//LogStream(LogStreamLevel::Info) << "Render Time [" << durr.count() << "]";

		mCurrent->logic.clear();
		// https://discourse.glfw.org/t/newbie-questions-trying-to-understand-glfwswapinterval/1287
		// Best practice is to default glfwSwapInterval to '1'
			// 0: do not wait for vsync(may be overridden by driver / driver settings)
			// 1 : wait for 1st vsync(may be overridden by driver / driver settings)
		glfwSwapInterval(mSwapInterval);
		glfwSwapBuffers(glfwWindow);
		do
		{
			glfwPollEvents();
			if (globals->minimised())
				std::this_thread::sleep_for(clamp);
		} while (globals->minimised());
		if (!mIsRunning)
		{ 
			glfwSetWindowShouldClose(glfwWindow, true);
		}
	}
}

void Movie::makeCurrent(const std::string& newSceneName)
{
	std::string safeSceneName = newSceneName;
	if (newSceneName == Scene::previousSceneName())
	{
		if (mPrevious)
		{
			safeSceneName = mPrevious->scene->name();
		}
		else
		{
			safeSceneName = Scene::finalSceneName();
		}
	}
	auto it = mScenes.find(safeSceneName);
	if (it == mScenes.end())
	{
		mIsRunning = false;
		safeSceneName = Scene::finalSceneName();
		LogStream(LogStreamLevel::LogicError) << "Unknown Scene Name [" << newSceneName
			<< "] passed to makeCurrent()\n";
	}
	makeCurrent(&(mScenes[safeSceneName]));

}

void Movie::makeCurrent(LoopControlStruct* lcs)
{
	if (mCurrent)
	{
		mCurrent->scene->deActivate(camera(), mCurrent->logic.current);
	}
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
	// TODO: inefficient. Used for debugging
	if (mUserInput.empty())
	{
		if (userInput.inputType == UserInput::InputAction::Press)
			mUserInput.push(userInput);
		else
			mUserInput.push(userInput);
	}
	else
	{
//		UserInput::AnyInput& front = mUserInput.front();
		if (userInput.inputType == UserInput::InputAction::Press)
			mUserInput.push(userInput);
//		else
//			mUserInput.push(userInput);
	}
}

void Movie::processUserInput(std::string& nextScene, OWUtils::Time::duration dt)
{
	OWUtils::Float timeStep = std::chrono::duration<float>(dt).count();

	while (!mUserInput.empty())
	{
		UserInput::AnyInput& input = mUserInput.front();
		LogStream(LogStreamLevel::Info) << "User Input ["
				<< input.keyInput.userCommand << "]\n";
		if (!mCurrent->logic.current->processUserCommands(input, nextScene, mCamera))
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
