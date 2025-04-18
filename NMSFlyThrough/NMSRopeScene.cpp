#include "NMSRopeScene.h"

#include <Cameras/Camera.h>
#include <Core/GlobalSettings.h>

#include <Component/TextComponent.h>
#include <Helpers/FreeTypeFontAtlas.h>
#include <Helpers/ShaderFactory.h>
#include <Component/LightSource.h>

#include "NMSUserInput.h"
#include "ropes.h"

Rope* gRope = nullptr;

NMSRopeScene::NMSRopeScene(const Movie* _movie)
	: NMSScene(_movie)
{
	//	const glm::uvec2& screen = globals->physicalWindowSize();
}

void NMSRopeScene::doSetupScene()
{
	const AABB& _world = NMSScene::world();
	OWRopeData rd;
	rd.bannerTextData.text = "Ropes";
	rd.ropeData.ropeZoom = { 500.0f * _world.size().x / globals->physicalWindowSize().x,
					500.0f * _world.size().y / globals->physicalWindowSize().y };
	rd.bannerTextData.fontHeight = 24;
	glm::vec2 spacing = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
		rd.bannerTextData.fontHeight, globals->camera()->aspectRatio());
	rd.bannerTextData.fontSpacing = spacing;
	glm::vec3 sc = { 5.2f * _world.size().x / globals->physicalWindowSize().x,
						5.2f * _world.size().y / globals->physicalWindowSize().y,
					1.0f };
	rd.ropeData.bannerTextScale = sc;
	rd.ropeData.labelTextScale = sc / 2.0f;
	rd.labelTextData.fontSpacing = spacing;
	/*
	* 30822 - simple
	* 29081 - strand for core
	* 9239 - Original used for testing
	*/
	rd.ropeData.ropeDBId = 9239;
#ifdef _DEBUG
	rd.ropeData.numDepthLayers = 45;
#else
	rd.ropeData.numDepthLayers = 45;// 1000;
#endif
	rd.ropeVisibility.ends = true;
	rd.ropeVisibility.lines = true;
	rd.ropeVisibility.surfaces = true;
	rd.ropeVisibility.strandLabels = true;
	rd.ropeVisibility.bannerLabel = true;
	gRope = new Rope(this, "Rope");
	gRope->initialise(rd);
	//LightSource* ls = new LightSource(new Physical({ 160.0f, 60.0f, 50.0f }), nullptr);
	//RendererBase* lightSource = NMS::createLightSource(glm::vec3(160.0f, 60.0f, 50.0f));
	//RendererBase* lightSource = NMS::createLightSource(glm::vec3(60.0f, 60.0f, -150.0f));
	//ls->prepare();
	//mRootNode->addChild(ls);
}

void NMSRopeScene::activate(const std::string& OW_UNUSED(previousScene),
	Camera* camera, unsigned int callCount)
{
	if (!callCount)
	{
		mCameraFocus = { 62.5595f, 62.5297, 0 };// gRope->bounds().center();
		mCameraFocus.z = -200;
		camera->position(mCameraFocus);
		mCameraFocus.z = 0;
		camera->lookAt(mCameraFocus);
		float speed = camera->moveScale();
		camera->moveScale(speed * 5.0f);
		//camera->FOV(glm::radians(45.0f));
		glm::mat4 view = camera->view();
		view = view;

	}
}

bool NMSRopeScene::processUserCommands(const UserInput::AnyInput& userInput,
	std::string& nextScene, Camera* OW_UNUSED(camera))
{
	if (userInput.inputType == UserInput::AnyInputType::Pointing)
	{
		// Mouse
	}
	else if ((userInput.inputType == UserInput::AnyInputType::KeyPress) &&
		(userInput.keyInput.action == UserInput::InputAction::Press))
	{
		// Keyboard
		if (userInput.keyInput.userCommand == NMSUserInput::LogicalOperator::OptionsScreen)
		{
			nextScene = Scene::finalSceneName();
			return true;
		}
		else if (userInput.keyInput.userCommand == NMSUserInput::LogicalOperator::Special1)
		{
			//const AABB& _world = NMSScene::world();
			//drawRope(_world);
			return true;
		}
		else if (userInput.keyInput.userCommand == NMSUserInput::LogicalOperator::OptionsScreen)
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

void NMSRopeScene::deActivate(const Camera* OW_UNUSED(camera))
{
}
/*
All keboard and model construction need to come from the physics class.All these classes need to somehow be
associated with a VAOBuffer to changes can be posted to the correct VAOBuffer.
Probably can drop HeavyRender and LightRender instead make a complex VAOBuffer class
Adding stuff to VAOBuffer needs to be posted to a queue and REnderer::setup can be done in another thread.
The Scene classes may end up very small with only the render function being virtual.

Have a RendererBase : has the queue which is then popped and associated with a VAOBuffer.
RendererBaseCollection: public RendererBase
E : \Apps\GamesEngines\OpenGL - Abstractions - This is good (I think has a good example of keyboard buffer)
Also https ://github.com/openscenegraph/OpenSceneGraph seems pretty good. (https://openscenegraph.org/projects/osg)
one of the E : \Apps\GamesEngines has a good example of mesh processing (which can be done in a background thread.)
Also see https ://help.autodesk.com/view/MAXDEV/2023/ENU/?guid=extracting_the_mesh_from_a_node for mesh processing
https://renderdoc.org/docs/getting_started/quick_start.html for debugging
https://docs.unity3d.com/Manual/ExecutionOrder.html
need to investigate http://gameprogrammingpatterns.com more
up to https://learnopengl.com/Lighting/Materials in second reading
also http ://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/ is quite good
https://blog.42yeah.is/
https://en.wikipedia.org/wiki/Scene_graph
*/
