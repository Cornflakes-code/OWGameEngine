#pragma once

#include "NMSScene.h"
#include "PolygonBuilder.h"
#include <Helpers/TextData.h>
#include <Renderers/Model.h>

/*
	An implementation of a Scene for the NMS game.
	Will be moved out of the engine to a different repo
*/
struct NMSRopeScenePhysics : public ScenePhysicsState
{
	glm::vec2 mNiceSpacing = { 0,0 };
	glm::vec2 mNiceScale = { 0,0 };
	TextData mTextData;
	std::pair<glm::vec3, glm::vec3> mMinMax = 
	{
		glm::vec3(std::numeric_limits<float>::min(),
				std::numeric_limits<float>::min(),
				std::numeric_limits<float>::min()),
		glm::vec3(std::numeric_limits<float>::max(),
				std::numeric_limits<float>::max(),
				std::numeric_limits<float>::max())
	};
	PolygonBuilder* mPolyBuilder = nullptr;
	std::vector<std::pair<TextData, glm::vec3>> mPolygonTextData;
	NMSRopeScenePhysics(const Scene* owner)
		: ScenePhysicsState(owner) {}
	void setup() override;
	void drawRope(const AABB& _world);
	void variableTimeStep(OWUtils::Time::duration dt) override;
	void fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt) override;
	void interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
		const ScenePhysicsState* currentState, double multCurr) override;
	bool processUserCommands(const UserInput::AnyInput& userInput,
		std::string& nextScene,
		Camera* camera) override;

	void copy(ScenePhysicsState* source) override;
	ScenePhysicsState* clone() override;

	float textPosition = 0.0;
};

class TextRenderer;

class NMSRopeScene : public NMSScene
{
	TextRenderer* mText = nullptr;
	std::vector<TextRenderer*> mPolyLabels;

	Model mWireLines;
	Model mWireEnds;
	Model mWireSurfaces;
	Model mLight;
	Model mCircles;
public:
	NMSRopeScene(const Movie* movie);
	std::string name() const { return "Rope"; }
	void doSetup(ScenePhysicsState* state) override;
	virtual void render(const ScenePhysicsState* state,
		const glm::mat4& proj, const glm::mat4& view,
		const glm::vec3& cameraPos) override;
	void activate(const std::string& previousScene, ScenePhysicsState* state,
		Camera* camera, unsigned int callCount) override;
	void deActivate(const Camera* camera, ScenePhysicsState* state) override;
};

