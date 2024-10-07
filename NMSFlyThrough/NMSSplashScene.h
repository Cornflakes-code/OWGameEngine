#pragma once

#include <limits>

#include <glm/glm.hpp>

#include <Core/BoundingBox.h>

#include <Helpers/MoveController.h>

#include <Helpers/MeshDataHeavy.h>
#include <Helpers/MeshDataInstance.h>
#include <Helpers/MeshDataLight.h>
#include <Helpers/TextData.h>

#include <Renderers/Model.h>
#include "NMSScene.h"
#include <Helpers/Button.h>

/*
	An implementation of a Scene for the NMS game.
*/

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
	ButtonData mButtonData;
	TextData mWelcomeData;
	TextData mEnjoyData;
	MoveController mWelcomeMover;
	MoveController mEnjoyMover;
	std::vector<glm::vec4> mFullScreenData;
	MeshDataInstance mStarData;
	glm::vec2 mStarRadius;
	MeshDataHeavy* mCylinderData;
};

class Axis;
class LightRenderer;
class InstanceRenderer;
class TextRenderer;
class HeavyRenderer;
class OWButton;

class NMSSplashScene : public NMSScene
{
	LightRenderer* mFullScreen = 0;
	InstanceRenderer* mStarRenderer = 0;
	TextRenderer* mWelcomeText = 0;
	TextRenderer* mEnjoyText = 0;
	HeavyRenderer* mCylinder = 0;
	Model mAxis;
	OWButton* mButton;
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

