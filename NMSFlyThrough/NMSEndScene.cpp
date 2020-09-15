#include "NMSEndScene.h"

#include <GLFW/glfw3.h>

#include <Renderables/TextBillboardDynamic.h>
#include <Helpers/Shader.h>
#include <Core/Camera.h>

#include "NMSUserInput.h"

void NMSEndScenePhysics::variableTimeStep(OWUtils::Time::duration dt)
{}

void NMSEndScenePhysics::fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt)
{}

void NMSEndScenePhysics::interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
	const ScenePhysicsState* currentState, double multCurr)
{}

void NMSEndScenePhysics::copy(ScenePhysicsState* source)
{
	*this = *(dynamic_cast<const NMSEndScenePhysics*>(source));
}

ScenePhysicsState* NMSEndScenePhysics::clone()
{
	return new NMSEndScenePhysics(owner());
}

bool NMSEndScenePhysics::processUserCommands(const UserInput::AnyInput& userInput, 
					std::string& nextScene,  const glm::uvec2& screenSize, 
					Camera* camera)
{
	if (userInput.inputType == UserInput::AnyInputType::Pointing)
	{
		// Mouse
	}
	else if (userInput.inputType == UserInput::AnyInputType::KeyPress)
	{
		// Keyboard
		NMSUserInput::NMSUserCommand input = 
			(NMSUserInput::NMSUserCommand) userInput.keyInput.userCommand;
		if (input == NMSUserInput::BaseUserCommand::OptionsScreen)
		{
			nextScene = Scene::quitSceneName();
			return true;
		}
		else if (input == NMSUserInput::BaseUserCommand::Accept)
		{
			nextScene = Scene::previousScene();
			return true;
		}
	}
	return false;
}

NMSEndScene::NMSEndScene(const Movie* _movie)
	: NMSScene(_movie)
{
	const glm::uvec2& screen = movie()->physicalWindowSize();
}

void NMSEndScene::doSetup(ScenePhysicsState* state)
{
	const float sx = 2.0f / movie()->physicalWindowSize().x;
	const float sy = 2.0f / movie()->physicalWindowSize().y;
	TextBillboard* txt = new TextBillboardDynamic("Arial.ttf", 24);

	glm::vec4 color(0.5, 0.8f, 0.2f, 0);
	txt->color(color);
	txt->createText("G", sx, sy);
	mText.setUp(txt);
}


void NMSEndScene::render(const ScenePhysicsState* state,
	const glm::mat4& proj, const glm::mat4& view)
{
	glm::mat4 model(1.0f);
	mText.render(proj, view, model);
}

void NMSEndScene::activate(const std::string& previousScene, ScenePhysicsState* state,
					Camera* camera, unsigned int callCount)
{
//	glBindVertexArray(text.VAO);
}

void NMSEndScene::deActivate(const std::string& previousScene, const Camera* camera, ScenePhysicsState* state)
{
}
