#pragma once

#include <vector>
#include <string>
#include <chrono>

#include "../OWEngine/OWEngine.h"

#include "Movie.h"
#include "UserInput.h"

/*
	A scene is a gross level of division in a game. For example a scene could be:
		1. The Welcome screen
		2. An Options Screens
		3. A Map keystrokes screen
		4. The main screen of the game. Almost all gameplay may exist inside this scene.
		5. A gross change in map location
	A scene change will involve a gross change in functionality and is often considered 
	an "out of game" experience.	
*/
class OWActor;
class OWCollider;

class OWENGINE_API Scene
{
public:
	static std::string previousSceneName() { return "Previous"; }
	static std::string finalSceneName() { return "Quit"; }
	virtual std::string name() const = 0;
	virtual void activate(const std::string& previousScene, 
				Camera* camera, unsigned int callCount) = 0;
	void addActor(OWActor* a);
	// Scene has been paged out. Scene may never come back so an 
	// opportunity to free resources maybe?
	virtual void deActivate(const Camera* camera) = 0;
	void setup();
	virtual bool processUserCommands(const UserInput::AnyInput& userInput,
		std::string& nextScene, Camera* camera) = 0;

	void cumulativeTime(const OWUtils::Time::duration& t) { mCumulativeTime = t; }
	OWUtils::Time::duration cumulativeTime() const { return mCumulativeTime; }

	void render(const glm::mat4& proj, const glm::mat4& view,
		const glm::vec3& cameraPos);
	const Movie* movie() const { return mMovie; }
	typedef std::function<void(OWActor* sc)> OWActorCallbackType;
	void traverseSceneGraph(OWActorCallbackType cb) const;
	void timeStep(std::string& nextScene, OWUtils::Time::duration fixedStep);
	void preRender(float totalTime, float alpha, float fixedTimeStep);
	void addCollider(OWCollider* coll, OWActor* a, int componentId);
protected:
	std::vector<OWActor*> mRootNode;
	Scene(const Movie* movie);
	virtual void doRenderScene(const glm::mat4& proj, const glm::mat4& view,
		const glm::vec3& cameraPos) {
	}
	virtual void doSetupScene() = 0;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	const Movie* mMovie = nullptr;
	OWUtils::Time::duration mCumulativeTime = std::chrono::milliseconds(0);
	bool mGetStateCalled = false;
#pragma warning( pop )
};