#include "QuitScene.h"

QuitScene::QuitScene(const Movie* movie)
	: Scene(movie)

{
}

void QuitScene::doSetupScene()
{
}

void QuitScene::activate(const std::string& OW_UNUSED(previousScene),
						 Camera* OW_UNUSED(camera),
						 unsigned int OW_UNUSED(callCount))
{
	movie()->close();
}

bool QuitScene::processUserCommands(const UserInput::AnyInput& userInput,
	std::string& nextScene, Camera* OW_UNUSED(camera))
{
	return false;
}

void QuitScene::deActivate(const Camera* OW_UNUSED(camera))
{
}
