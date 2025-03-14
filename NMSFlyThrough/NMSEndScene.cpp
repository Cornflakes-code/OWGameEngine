#include "NMSEndScene.h"

#include <Cameras/Camera.h>
#include <Core/GlobalSettings.h>
#include <Actor/OWActor.h>

#include <Helpers/Shader.h>
#include <Component/TextComponent.h>

#include "NMSUserInput.h"

void NMSEndScenePhysics::setup()
{

}

void NMSEndScenePhysics::variableTimeStep(OWUtils::Time::duration OW_UNUSED(dt))
{}

void NMSEndScenePhysics::fixedTimeStep(std::string& OW_UNUSED(nextSceneName), 
										OWUtils::Time::duration OW_UNUSED(dt))
{}

void NMSEndScenePhysics::interpolateRatio(const ScenePhysicsState* OW_UNUSED(previousState),
								double OW_UNUSED(multPrev), 
								const ScenePhysicsState* OW_UNUSED(currentState),
								double OW_UNUSED(multCurr))
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
					std::string& nextScene, Camera* OW_UNUSED(camera))
{
	if (userInput.inputType == UserInput::AnyInputType::Pointing)
	{
		// Mouse
	}
	else if (userInput.inputType == UserInput::AnyInputType::KeyPress)
	{
		// Keyboard
		if (userInput.keyInput.userCommand == NMSUserInput::LogicalOperator::OptionsScreen)
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

NMSEndScene::NMSEndScene(const Movie* _movie)
	: NMSScene(_movie)
{
//	const glm::uvec2& screen = globals->physicalWindowSize();
}

void NMSEndScene::doSetupScene(ScenePhysicsState* state)
{
	const float sx = 2.0f / globals->physicalWindowSize().x;
	const float sy = 2.0f / globals->physicalWindowSize().y;
	// Needs an OWActor as owner and a position
	//TextComponent* td = new TextComponent("G", TextComponent::Dynamic, sx, sy);
	//td->colour( { 0.5, 0.8f, 0.2f, 0 } );
	//td->prepare();
	//mRootNode->addChild(td);
}

void NMSEndScene::activate(const std::string& OW_UNUSED(previousScene),
					ScenePhysicsState* OW_UNUSED(state),
					Camera* OW_UNUSED(camera), unsigned int OW_UNUSED(callCount))
{
}

void NMSEndScene::deActivate(const Camera* OW_UNUSED(camera), 
							 ScenePhysicsState* OW_UNUSED(state))
{
}
