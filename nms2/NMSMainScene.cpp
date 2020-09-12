#include "NMSMainScene.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_projection.hpp>

#include <Core/Movie.h>

#include <Helpers/ErrorHandling.h>
#include <Core/Camera.h>
#include <Renderables/Axis.h>

#include "NMSUserInput.h"
#include "NoMansSkyStarMap.h"

void NMSMainScenePhysics::variableTimeStep(OWUtils::Time::duration dt)
{}

void NMSMainScenePhysics::fixedTimeStep(std::string& nextSceneName, OWUtils::Time::duration dt)
{}

void NMSMainScenePhysics::interpolateRatio(const ScenePhysicsState* previousState, double multPrev,
	const ScenePhysicsState* currentState, double multCurr)
{}

void NMSMainScenePhysics::copy(ScenePhysicsState* source)
{
	*this = *(dynamic_cast<const NMSMainScenePhysics*>(source));
}

ScenePhysicsState* NMSMainScenePhysics::clone() 
{
	return new NMSMainScenePhysics(owner());
}

bool NMSMainScenePhysics::processUserCommands(const UserInput::AnyInput& userInput, 
				std::string& nextScene, const glm::uvec2& screenSize, 
				Camera* camera)
{
	if (userInput.inputType == UserInput::AnyInputType::Pointing)
	{
		/*
			Links re converting Screen Coords to World Coords
			https://stackoverflow.com/questions/45796287/screen-coordinates-to-world-coordinates
		// https://stackoverflow.com/questions/9901453/using-glms-unproject
		*/
		glm::vec4 viewportdata = glm::vec4(0.0f, 0.0f, screenSize.x, screenSize.y);
		glm::mat4 projection = camera->projection();
		glm::mat4 view = camera->view();
		glm::mat4 model = glm::mat4(1.0);

		const Scene* ow = owner();
		glm::vec3 un 
			= glm::unProject(userInput.mouseInput.pos, 
					view * model, projection, viewportdata);
	}
	else
	{
		// Keyboard
		NMSUserInput::BaseUserCommand input 
			= (NMSUserInput::BaseUserCommand) userInput.keyInput.userCommand;
		if (input == NMSUserInput::BaseUserCommand::OptionsScreen)
		{
			nextScene = NMSScene::endSceneName();
			return true;
		}
	}
	return false;
}

NMSMainScene::NMSMainScene(const Movie* movie)
	: NMSScene(movie)
{
}

void NMSMainScene::doSetup(ScenePhysicsState* state)
{
	NMSMainScenePhysics* sp = dynamic_cast<NMSMainScenePhysics*>(state);
	sp->mCameraPosition = movie()->camera()->position();
	sp->mLookAt = { 0,0,0 };

	mStarMap = new NoMansSky();
	mStarMap->setUp("../nms2/NMSMap.txt", world());
	mAxis = new Axis();
	
	mAxis->setUp(world(), movie()->camera());
}

void NMSMainScene::render(const ScenePhysicsState* state,
						  const glm::mat4& proj, const glm::mat4& view)
{
	glm::mat4 model(1.0);
	mAxis->render(proj, view, model);
	mStarMap->render(proj, view, model);
}

void NMSMainScene::activate(const std::string& previousScene, ScenePhysicsState* state,
					Camera* camera, unsigned int callCount)
{
	NMSMainScenePhysics* sp = dynamic_cast<NMSMainScenePhysics*>(state);
	if (!callCount)
	{
		camera->position(sp->mCameraPosition);
		camera->lookAt(sp->mLookAt);
	}
}

void NMSMainScene::deActivate(const std::string& previousScene, const Camera* camera, ScenePhysicsState* state)
{
	NMSMainScenePhysics* sp = dynamic_cast<NMSMainScenePhysics*>(state);
	sp->mCameraPosition = camera->position();
	sp->mLookAt = camera->lookAt();
}
