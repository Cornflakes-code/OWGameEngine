#include "NMSEndScene.h"

#include <Cameras/Camera.h>
#include <Core/GlobalSettings.h>
#include <Actor/OWActor.h>

#include <Helpers/Shader.h>
#include <Component/TextComponent.h>

#include "NMSUserInput.h"



NMSEndScene::NMSEndScene(const Movie* _movie)
	: NMSScene(_movie)
{
//	const glm::uvec2& screen = globals->physicalWindowSize();
}

void NMSEndScene::doSetupScene()
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
					Camera* OW_UNUSED(camera), unsigned int OW_UNUSED(callCount))
{
}

void NMSEndScene::deActivate(const Camera* OW_UNUSED(camera))
{
}

bool NMSEndScene::processUserCommands(const UserInput::AnyInput& userInput,
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
