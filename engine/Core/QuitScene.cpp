#include "QuitScene.h"

void QuitScenePhysics::copy(ScenePhysicsState* source)
{
	*this = *(dynamic_cast<const QuitScenePhysics*>(source));
}

ScenePhysicsState* QuitScenePhysics::clone()
{
	return new QuitScenePhysics(owner());
}

bool QuitScenePhysics::processUserCommands(const UserInput::AnyInput& OW_UNUSED(userInput),
				std::string& OW_UNUSED(nextScene),
				Camera* OW_UNUSED(camera))
{
	return false;
}

QuitScene::QuitScene(const Movie* movie)
	: Scene(movie)

{
}

void QuitScene::doSetupScene(ScenePhysicsState* state)
{
}

void QuitScene::activate(const std::string& OW_UNUSED(previousScene),
						 ScenePhysicsState* OW_UNUSED(state),
						 Camera* OW_UNUSED(camera),
						 unsigned int OW_UNUSED(callCount))
{
	movie()->close();
}

void QuitScene::deActivate(const Camera* OW_UNUSED(camera),
						   ScenePhysicsState* OW_UNUSED(state))
{
}
