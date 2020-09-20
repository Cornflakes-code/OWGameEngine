#pragma once

#include "../OWEngine/OWEngine.h"

#include "Scene.h"
/*
	A core Scene required as a placeholder for the Game Loop implementation in Movie.cpp
*/
struct OWENGINE_API QuitScenePhysics: public ScenePhysicsState
{
	QuitScenePhysics(const Scene* owner)
		: ScenePhysicsState(owner) {}
	void variableTimeStep(OWUtils::Time::duration OW_UNUSED(dt)) override {}
	void fixedTimeStep(std::string& OW_UNUSED(nextSceneName),
					   OWUtils::Time::duration OW_UNUSED(dt)) override {}
	void interpolateRatio(const ScenePhysicsState* OW_UNUSED(previousState),
						  double OW_UNUSED(multPrev),
						  const ScenePhysicsState* OW_UNUSED(currentState),
						  double OW_UNUSED(multCurr)) override {}
	bool processUserCommands(const UserInput::AnyInput& OW_UNUSED(userInput),
							 std::string& OW_UNUSED(nextScene),
							 Camera* OW_UNUSED(camera)) override;

	void copy(ScenePhysicsState* OW_UNUSED(source)) override;
	ScenePhysicsState* clone() override;
};


class QuitScene : public Scene
{
	// The framework requires a scene with a name of OWUtils::quitSceneName().
	// It will be the last scene loaded and can (not must!) be overridden to clear resource.
public:
	QuitScene(const Movie* movie);
	virtual std::string name() const final { return quitSceneName(); }
	void doSetup(ScenePhysicsState* state) override;
	virtual void render(const ScenePhysicsState* state,
		const glm::mat4& proj, const glm::mat4& view) override;
	void activate(const std::string& previousScene, ScenePhysicsState* state,
				  Camera* camera, unsigned int callCount) override;
	void deActivate(const std::string& previousScene, const Camera* camera, ScenePhysicsState* state) override;
};
