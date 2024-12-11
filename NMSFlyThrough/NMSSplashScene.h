#pragma once

#include <limits>

#include <glm/glm.hpp>

#include <Core/BoundingBox.h>

#include "NMSScene.h"

/*
	An implementation of a Scene for the NMS game.
*/

class TextData;
struct ButtonData;
class MeshDataInstance;

struct NMSSplashScenePhysics: public NMSWorldPhysicsState
{
	NMSSplashScenePhysics(const Scene* owner)
		: NMSWorldPhysicsState(owner) {}

	void setup() override;
	virtual void variableTimeStep(OWUtils::Time::duration dt) override;
	virtual void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) override;
	virtual void interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
		const ScenePhysicsState* currentState, double multCurr) override;
	bool processUserCommands(const UserInput::AnyInput& userInput, 
							 std::string& nextScene, 
							 Camera* camera) override;

	ScenePhysicsState* clone() override;
	void copy(ScenePhysicsState* source) override;
	void clear() override;

	static AABB mWindowBounds;
	static OWUtils::Float mSpeed;
	ButtonData* mButtonData = nullptr;
	TextData* mWelcome = nullptr;
	TextData* mEnjoy = nullptr;
	MeshDataInstance* mStarData = nullptr;
	glm::vec2 mStarRadius = glm::vec2(0);
};

class Axis;
class InstanceRenderer;
class TextRenderer;
class HeavyRenderer;
class OWButton;

class NMSSplashScene : public NMSScene
{
	InstanceRenderer* mStarRenderer = nullptr;
	OWButton* mButton = nullptr;
public:
	NMSSplashScene(const Movie* movie);
	std::string name() const { return "Splash"; }
	void doSetup(ScenePhysicsState* state) override;
	virtual void render(const ScenePhysicsState* state,
		const glm::mat4& proj, const glm::mat4& view,
		const glm::vec3& cameraPos) override;
	void activate(const std::string& previousScene, 
				  ScenePhysicsState* state,
				  Camera* camera, unsigned int callCount) override;
	void deActivate(const Camera* camera, 
					ScenePhysicsState* state) override;
private:
};

