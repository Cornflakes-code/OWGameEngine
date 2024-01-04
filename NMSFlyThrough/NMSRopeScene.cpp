#include "NMSRopeScene.h"

#include <Core/Camera.h>
#include <Core/GlobalSettings.h>

#include <Helpers/Shader.h>
#include <Helpers/TextData.h>
#include <Helpers/FreeTypeFontAtlas.h>
#include <Helpers/MeshDataLight.h>
#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>

#include <Renderers/TextRendererDynamic.h>
#include <Renderers/LightRenderer.h>

#include "NMSUserInput.h"
#include "rope_interface_utils.h"
#include "rope_interface_test.h"
#include "rope_quick.h"
#include "ropes.h"
#include "NMSUtils.h"

void NMSRopeScenePhysics::setup()
{
	const AABB& _world = NMSScene::world();
	int fontHeight = 24;
	glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
		fontHeight, globals->camera()->aspectRatio());
	glm::vec2 scale = { 5.2f * _world.size().x / globals->physicalWindowSize().x,
						5.2f * _world.size().y / globals->physicalWindowSize().y };

	mTextData.text("Ropes.");
	mTextData.font("arial.ttf", fontHeight);
	mTextData.colour({ 0.0, 0.0, 0.0, 1.0f });
	mTextData.spacing(10 * nice.x, 10 * nice.y, scale);

	createRopes();

	mVectors.clear();
	glm::vec2 ropeZoom = { 500.0f * _world.size().x / globals->physicalWindowSize().x,
					500.0f * _world.size().y / globals->physicalWindowSize().y };

	PolygonBuilder* pb = drawRope(9239, ropeZoom.x, ropeZoom.y);
	pb->getAllFloats(mVectors);
}

void NMSRopeScenePhysics::variableTimeStep(OWUtils::Time::duration OW_UNUSED(dt))
{}

void NMSRopeScenePhysics::fixedTimeStep(std::string& OW_UNUSED(nextSceneName),
	OWUtils::Time::duration OW_UNUSED(dt))
{}

void NMSRopeScenePhysics::interpolateRatio(const ScenePhysicsState* OW_UNUSED(previousState),
	double OW_UNUSED(multPrev),
	const ScenePhysicsState* OW_UNUSED(currentState),
	double OW_UNUSED(multCurr))
{}

void NMSRopeScenePhysics::copy(ScenePhysicsState* source)
{
	*this = *(dynamic_cast<const NMSRopeScenePhysics*>(source));
}

ScenePhysicsState* NMSRopeScenePhysics::clone()
{
	return new NMSRopeScenePhysics(owner());
}

bool NMSRopeScenePhysics::processUserCommands(const UserInput::AnyInput& userInput,
	std::string& nextScene, Camera* OW_UNUSED(camera))
{
	if (userInput.inputType == UserInput::AnyInputType::Pointing)
	{
		// Mouse
	}
	else if ((userInput.inputType == UserInput::AnyInputType::KeyPress) &&
			(userInput.keyInput.action == UserInput::InputAction::Press))
	{
		// Keyboard
		if (userInput.keyInput.userCommand == NMSUserInput::LogicalOperator::OptionsScreen)
		{
			nextScene = Scene::finalSceneName();
			return true;
		}
		else if (userInput.keyInput.userCommand == NMSUserInput::LogicalOperator::Special1)
		{
			mVectors.clear();
			PolygonBuilder* pb = drawRope(9239, 5, 5);
			pb->getAllFloats(mVectors);
			return true;
		}
		else if (userInput.keyInput.userCommand == NMSUserInput::LogicalOperator::OptionsScreen)
		{
			nextScene = Scene::finalSceneName();
			return true;
		}
		else if (userInput.keyInput.userCommand == NMSUserInput::LogicalOperator::Accept)
		{
			nextScene = Scene::previousSceneName();
			return true;
		}
	}
	return false;
}

NMSRopeScene::NMSRopeScene(const Movie* _movie)
	: NMSScene(_movie)
{
	//	const glm::uvec2& screen = globals->physicalWindowSize();
}

void NMSRopeScene::doSetup(ScenePhysicsState* state)
{
	NMSRopeScenePhysics* sps
		= dynamic_cast<NMSRopeScenePhysics*>(state);
	mText = new TextRendererDynamic();
	mText->setup(&(sps->mTextData), glm::vec3(0));

	ModelData md;
	mAxis.setup(&md);

	ShaderFactory shaders;
	Shader* lineShader = new Shader();
	lineShader->loadShaders(shaders.boilerPlateVertexShader(),
		shaders.boilerPlateFragmentShader(),
		shaders.boilerPlateGeometryShader());

	std::vector<glm::vec3> coords;
	for (auto& val : sps->mVectors)
	{
		MeshDataLight lineData;
		lineData.colour(OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN), "colour");
		lineData.vertices(val, GL_LINES);
		LightRenderer* lines = new LightRenderer(lineShader, "pvm");
		lines->setup(&lineData);
		md.renderers.push_back(lines);
	}
	mCircles.setup(&md);
}

void NMSRopeScene::render(const ScenePhysicsState* OW_UNUSED(state),
	const glm::mat4& proj, const glm::mat4& view)
{
	glm::mat4 model(1.0f);
	mText->render(proj, view, model);
	mAxis.render(proj, view, model);
	mCircles.render(proj, view, model);
}

void NMSRopeScene::activate(const std::string& OW_UNUSED(previousScene),
	ScenePhysicsState* OW_UNUSED(state),
	Camera* OW_UNUSED(camera), unsigned int OW_UNUSED(callCount))
{
}

void NMSRopeScene::deActivate(const Camera* OW_UNUSED(camera),
	ScenePhysicsState* OW_UNUSED(state))
{
}
