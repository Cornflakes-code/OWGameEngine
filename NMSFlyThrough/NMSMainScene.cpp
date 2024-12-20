#include "NMSMainScene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_projection.hpp>

#include <Core/Camera.h>
#include <Core/GLApplication.h>
#include <Core/GlobalSettings.h>
#include <Core/ResourcePathFactory.h>

#include <Helpers/ThreeDAxis.h>

#include "NMSUserInput.h"
#include "NoMansSkyStarMap.h"
#include "NMSEndScene.h"

void NMSMainScenePhysics::setup()
{

}

void NMSMainScenePhysics::variableTimeStep(OWUtils::Time::duration OW_UNUSED(dt))
{}

void NMSMainScenePhysics::fixedTimeStep(std::string& OW_UNUSED(nextSceneName), 
										OWUtils::Time::duration OW_UNUSED(dt))
{}

void NMSMainScenePhysics::interpolateRatio(const ScenePhysicsState* OW_UNUSED(previousState), 
							double OW_UNUSED(multPrev),
							const ScenePhysicsState* OW_UNUSED(currentState), 
							double OW_UNUSED(multCurr))
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
				std::string& nextScene, Camera* camera)
{
	if (userInput.inputType == UserInput::AnyInputType::Pointing)
	{
		/*
			Links re converting Screen Coords to World Coords
			https://stackoverflow.com/questions/45796287/screen-coordinates-to-world-coordinates
		// https://stackoverflow.com/questions/9901453/using-glms-unproject
		*/
		glm::vec4 viewportdata = glm::vec4(0.0f, 0.0f, 
				globals->physicalWindowSize().x, 
				globals->physicalWindowSize().y);
		glm::mat4 projection = camera->projection();
		glm::mat4 view = camera->view();
		glm::mat4 model = glm::mat4(1.0);

		glm::vec3 un 
			= glm::unProject(userInput.mouseInput.pos, 
					view * model, projection, viewportdata);
	}
	else
	{
		// Keyboard
		if (userInput.keyInput.userCommand == NMSUserInput::LogicalOperator::OptionsScreen)
		{
			nextScene = Scene::finalSceneName();
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

	NoMansSky* starMap = new NoMansSky(this, glm::vec3(0));
	std::filesystem::path p
		= ResourcePathFactory().appendPath("NMSMap.txt", 
				ResourcePathFactory::ResourceType::UnknownType);
	starMap->setUp(p.string(), world());
	mRootNode.push_back(starMap);
	ThreeDAxis* axis = new ThreeDAxis(this, glm::vec3(0));
	axis->createAxisData(world());
	mRootNode.push_back(axis);
}

void NMSMainScene::render(const ScenePhysicsState* state,
						  const glm::mat4& proj, const glm::mat4& view, 
						const glm::vec3& cameraPos)
{
	glm::mat4 model(1.0);

	auto rend = [proj, view, model, cameraPos](OWActor* a)
	{
		a->render(proj, view, model, cameraPos);
	};
	traverseSceneGraph(rend);
}

void NMSMainScene::activate(const std::string& OW_UNUSED(previousScene), 
							ScenePhysicsState* state,
							Camera* camera, unsigned int callCount)
{
	NMSMainScenePhysics* sp = dynamic_cast<NMSMainScenePhysics*>(state);
	if (!callCount)
	{
		camera->position(sp->mCameraPosition);
		camera->lookAt(sp->mLookAt);
	}
}

void NMSMainScene::deActivate(const Camera* camera, ScenePhysicsState* state)
{
	NMSMainScenePhysics* sp = dynamic_cast<NMSMainScenePhysics*>(state);
	sp->mCameraPosition = camera->position();
	sp->mLookAt = camera->lookAt();
}
