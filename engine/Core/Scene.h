#pragma once

#include <vector>
#include <string>
#include <chrono>

#include "../OWEngine/OWEngine.h"
#include "../Core/Actor.h"

#include "ScenePhysicsState.h"
#include "Movie.h"
#include "UserInput.h"

/*
	My implementation of a Fixed timestep physics/Variable timestep Render Game
	loop requires the separation of static and dynamic physics. Class Scene
	provides the base class for the static information and class ScenePhysicsState
	provides the base class for the dynamic information.
	Class Scene is the base class for the invariant state information. Think of Scene
	it as providing the DOM in a Web Page while class ScenePhysicsState provides the
	cookies. Class Scene (and children) will be heavy on immutable objects, low on
	functionality. When rendering, class Scene reads the state info in class 
	ScenePhysicsState.

	A scene is a gross level of division in a game. For example a scene could be:
		1. The Welcome screen
		2. An Options Screens
		3. A Map keystrokes screen
		4. The main screen of the game. Almost all gameplay may exist inside this scene.
		5. A gross change in map location
	A scene change will involve a gross change in functionality and is often considered 
	an "out of game" experience.	
*/
class OWENGINE_API Scene
{
public:
	static std::string previousSceneName() { return "Previous"; }
	static std::string finalSceneName() { return "Quit"; }
	virtual std::string name() const = 0;
	virtual void activate(const std::string& previousScene, 
	ScenePhysicsState* state, Camera* camera, unsigned int callCount) = 0;

	// Scene has been paged out. Scene may never come back so an 
	// opportunity to free resources maybe?
	virtual void deActivate(const Camera* camera, ScenePhysicsState* state) = 0;
	void setup(ScenePhysicsState* state);

	void cumulativeTime(const OWUtils::Time::duration& t) { mCumulativeTime = t; }
	OWUtils::Time::duration cumulativeTime() const { return mCumulativeTime; }

	virtual void render(const ScenePhysicsState* state,
						const glm::mat4& proj, const glm::mat4& view,
						const glm::vec3& cameraPos) = 0;
	const Movie* movie() const { return mMovie; }
	SceneGraphNode* mRootNode = nullptr;
protected:
	Scene(const Movie* movie);

	virtual void doSetup(ScenePhysicsState* state) = 0;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	const Movie* mMovie = nullptr;
	OWUtils::Time::duration mCumulativeTime = std::chrono::milliseconds(0);
	bool mGetStateCalled = false;
#pragma warning( pop )
};