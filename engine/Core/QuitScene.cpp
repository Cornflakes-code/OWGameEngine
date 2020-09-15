#include "QuitScene.h"

void QuitScenePhysics::copy(ScenePhysicsState* source)
{
	*this = *(dynamic_cast<const QuitScenePhysics*>(source));
}

ScenePhysicsState* QuitScenePhysics::clone()
{
	return new QuitScenePhysics(owner());
}

bool QuitScenePhysics::processUserCommands(const UserInput::AnyInput& userInput,
				std::string& nextScene, 
				const glm::uvec2& screenSize, 
				Camera* camera)
{
	return false;
}

QuitScene::QuitScene(const Movie* movie)
	: Scene(movie)

{
}

void QuitScene::doSetup(ScenePhysicsState* state)
{
}


void QuitScene::render(const ScenePhysicsState* state,
	const glm::mat4& proj, const glm::mat4& view)
{
}

void QuitScene::activate(const std::string& previousScene, ScenePhysicsState* state,
					Camera* camera, unsigned int callCount)
{
	glfwSetWindowShouldClose(movie()->window(), true);
}

void QuitScene::deActivate(const std::string& previousScene, const Camera* camera, ScenePhysicsState* state)
{
}
