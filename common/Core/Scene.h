#pragma once

#include <vector>
#include <string>
#include <chrono>

#include "ScenePhysicsState.h"
#include "Movie.h"
#include "UserInput.h"

class Scene
{
public:
	static std::string quitSceneName() { return "Options"; }
	static std::string previousScene() { return "previous"; }
	virtual std::string name() const = 0;
	virtual void activate(const std::string& previousScene, 
	ScenePhysicsState* state, Camera* camera, unsigned int callCount) = 0;

	// Scene has been paged out. Scene may never come back so an 
	// opportunity to free resources maybe?
	virtual void deActivate(const std::string& previousScene, 
					const Camera* camera, ScenePhysicsState* state) = 0;
	void setup(ScenePhysicsState* state);

	void cumulativeTime(const OWUtils::Time::duration& t) { mCumulativeTime = t; }
	OWUtils::Time::duration cumulativeTime() const { return mCumulativeTime; }

	virtual void render(const ScenePhysicsState* state,
						const glm::mat4& proj, const glm::mat4& view) = 0;
	const Movie* movie() const { return mMovie; }
protected:
	Scene(const Movie* movie);

	virtual void doSetup(ScenePhysicsState* state) = 0;
private:
	bool mGetStateCalled = false;
	const Movie* mMovie = nullptr;
	OWUtils::Time::duration mCumulativeTime = std::chrono::milliseconds(0);
};