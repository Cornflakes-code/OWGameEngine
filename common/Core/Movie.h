#pragma once

#include <map>
#include <functional>
#include <chrono>
#include <string>
#include <queue>

#include "../Renderables/BoundingBox.h"

#include "ScenePhysicsState.h"
#include "UserInput.h"


class Scene;
class Camera;
class Logger;
struct GLFWwindow;
class UserInput;
class GLApplication;
class MacroRecorder;

class Movie
{
public:
	virtual void init(GLApplication* app, UserInput* ui, MacroRecorder* recorder);
	virtual void preRun();
	void run(UserInput* ui);

	const glm::uvec2& physicalWindowSize() const { return mPhysicalWindowSize; }
	void physicalWindowSize(const glm::uvec2& newSize);
	virtual std::string windowTitle() const { return mWindowTitle; }
	void window(GLFWwindow* glfw) { mWindow = glfw; }
	GLFWwindow* window() const { return mWindow; }
	const Camera* camera() const { return mCamera; }
	Camera* camera() { return mCamera; }
protected:
	Movie(const std::string& _windowTitle, 
		  const glm::uvec2& _windowSize, 
		  Camera* _camera);

	void add(Scene* toAdd, ScenePhysicsState* sps, bool makeThisSceneCurrent = false);
	virtual void render(const ScenePhysicsState* state);
	void pushUserInput(const UserInput::AnyInput& anyInput);
private:
	std::queue<UserInput::AnyInput> mUserInput;
	Logger* mLogger;
	std::string mWindowTitle;
	glm::uvec2 mPhysicalWindowSize;
	Camera* mCamera = nullptr;

	struct LoopControlStruct
	{
		unsigned int countActivateCalled = 0;
		bool setupCalled = false;
		Scene* scene = nullptr;
		SceneLogic logic;
	};

	std::map<std::string, LoopControlStruct> mScenes;
	LoopControlStruct* mCurrent = nullptr;
	LoopControlStruct* mPrevious = nullptr;
	GLFWwindow* mWindow = nullptr;

	void makeCurrent(LoopControlStruct* lcs);
	void makeCurrent(const std::string& newSceneName);

	// return true if scene changed from this
	void processUserInput(std::string& nextScene, OWUtils::Time::duration fixedStep);
	virtual bool canResize() const { return true; }

	void camera(Camera* newValue) { mCamera = newValue; }
};
