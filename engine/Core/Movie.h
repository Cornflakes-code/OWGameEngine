#pragma once

#include <map>
#include <functional>
#include <chrono>
#include <string>
#include <queue>

#include "../OWEngine/OWEngine.h"
#include "../Renderables/BoundingBox.h"

#include "ScenePhysicsState.h"


class Scene;
class Camera;
class Logger;
struct GLFWwindow;
class UserInput;
class GLApplication;
class MacroRecorder;
/*
	Core class providing the main Game loop. Tightly bound to the Scene and
	ScenePhysicsState classes
*/
class OWENGINE_API Movie
{
public:
	virtual void init(GLApplication* app, UserInput* ui, MacroRecorder* recorder);
	virtual void preRun();
	void run(UserInput* ui, GLFWwindow* glfw);
	virtual std::string windowTitle() const { return mWindowTitle; }
	const Camera* camera() const { return mCamera; }
	Camera* camera() { return mCamera; }
	void close() const { mIsRunning = false; }
protected:
	Movie(const std::string& _windowTitle, Camera* _camera);

	void add(Scene* toAdd, ScenePhysicsState* sps, bool makeThisSceneCurrent = false);
	virtual void render(const ScenePhysicsState* state);
	void pushUserInput(const UserInput::AnyInput& anyInput);
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	struct LoopControlStruct
	{
		Scene* scene = nullptr;
		SceneLogic logic;
		unsigned int countActivateCalled = 0;
		bool setupCalled = false;
	};
	Logger* mLogger;
	Camera* mCamera = nullptr;
	LoopControlStruct* mCurrent = nullptr;
	LoopControlStruct* mPrevious = nullptr;

	std::string mWindowTitle;
	std::queue<UserInput::AnyInput> mUserInput;
	std::map<std::string, LoopControlStruct> mScenes;
	mutable bool mIsRunning = true;

	void makeCurrent(LoopControlStruct* lcs);
	void makeCurrent(const std::string& newSceneName);

	// return true if scene changed from this
	void processUserInput(std::string& nextScene, OWUtils::Time::duration fixedStep);
	virtual bool canResize() const { return true; }

	void camera(Camera* newValue) { mCamera = newValue; }
#pragma warning( pop )
};
