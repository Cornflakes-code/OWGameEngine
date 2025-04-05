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

NMSMainScene::NMSMainScene(const Movie* movie)
	: NMSScene(movie)
{
}

void NMSMainScene::doSetupScene()
{
	mCameraPosition = movie()->camera()->position();
	mLookAt = { 0,0,0 };
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
							Camera* camera, unsigned int callCount)
{
	if (!callCount)
	{
		camera->position(mCameraPosition);
		camera->lookAt(mLookAt);
	}
}

bool NMSMainScene::processUserCommands(const UserInput::AnyInput& userInput, std::string& nextScene, Camera* camera)
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


void NMSMainScene::deActivate(const Camera* camera)
{
	mCameraPosition = camera->position();
	mLookAt = camera->lookAt();
}
