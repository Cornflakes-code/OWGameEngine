#pragma once

#include <limits>

#include <glm/glm.hpp>

#include <Geometry/BoundingBox.h>

#include "NMSScene.h"

/*
	An implementation of a Scene for the NMS game.
*/

class TextComponent;
struct ButtonData;
class MeshDataInstance;

struct NMSSplashScenePhysics: public NMSWorldPhysicsState
{
	NMSSplashScenePhysics(Scene* owner);

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
	MeshDataInstance* mStarData = nullptr;
	glm::vec2 mStarRadius = glm::vec2(0);
};

class Axis;
class HeavyRenderer;
class OLDButton;

class NMSSplashScene : public NMSScene
{
	OLDButton* mButton = nullptr;
public:
	NMSSplashScene(const Movie* movie);
	std::string name() const { return "Splash"; }
	virtual void doRenderScene(const ScenePhysicsState* state,
		const glm::mat4& proj, const glm::mat4& view,
		const glm::vec3& cameraPos) override;
	void activate(const std::string& previousScene, 
				  ScenePhysicsState* state,
				  Camera* camera, unsigned int callCount) override;
	void deActivate(const Camera* camera, 
					ScenePhysicsState* state) override;
protected:
	void doSetupScene(ScenePhysicsState* state) override;
private:
};

