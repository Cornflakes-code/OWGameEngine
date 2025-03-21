#include "NMSMainScene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_projection.hpp>

#include <Cameras/Camera.h>
#include <Core/GLApplication.h>
#include <Core/GlobalSettings.h>
#include <Core/ResourcePathFactory.h>

#include <Actor/ThreeDAxis.h>

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

void NMSMainScene::doSetupScene(ScenePhysicsState* state)
{
	NMSMainScenePhysics* sp = dynamic_cast<NMSMainScenePhysics*>(state);
	sp->mCameraPosition = movie()->camera()->position();
	sp->mLookAt = { 0,0,0 };
	NoMansSkyData nmsd;
		
	nmsd.starFile = ResourcePathFactory().appendPath("NMSMap.txt",
		ResourcePathFactory::ResourceType::UnknownType).string();
	nmsd.starWorld = world();
	nmsd.name = "grid";
#ifdef _DEBUG
	// With VBO's about 1000 FPS
	nmsd.numberOfStars = 50000;
#else
	// With VBO's about 3000 FPS
	nmsd.numberOfStars = 50000;
#endif
	NoMansSky* nms = new NoMansSky(this, "NMS");
	nms->initialise(nmsd);
	OWThreeDAxisData threeDAxisData;
	threeDAxisData.bounds = world();
	threeDAxisData.labelColour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_WHITE);
	ThreeDAxis* threeDAxis = new ThreeDAxis(this, "NMS Axis");
	threeDAxis->initialise(threeDAxisData);
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
