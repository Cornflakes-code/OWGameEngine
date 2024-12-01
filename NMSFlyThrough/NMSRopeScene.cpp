#include "NMSRopeScene.h"

#include <Core/Camera.h>
#include <Core/GlobalSettings.h>

#include <Helpers/Shader.h>
#include <Helpers/TextData.h>
#include <Helpers/FreeTypeFontAtlas.h>
#include <Helpers/MeshDataLight.h>
#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>
#include <Helpers/LightSource.h>

#include "NMSUserInput.h"
#include "rope_interface_utils.h"
#include "rope_interface_test.h"
#include "rope_quick.h"
#include "ropes.h"

void NMSRopeScenePhysics::setup()
{
	Rope* rope = new Rope(nullptr);
	rope->prepare();
	const AABB& _world = NMSScene::world();
	glm::vec2 ropeZoom = { 500.0f * _world.size().x / globals->physicalWindowSize().x,
					500.0f * _world.size().y / globals->physicalWindowSize().y };
	int fontHeight = 24;
	glm::vec2 niceTextSpacing = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
		fontHeight, globals->camera()->aspectRatio());
	glm::vec2 niceTextScale = { 5.2f * _world.size().x / globals->physicalWindowSize().x,
						5.2f * _world.size().y / globals->physicalWindowSize().y };
	/*
	* 30822 - simple
	* 29081 - strand for core
	* 9239 - Original used for testing
	*/
	rope->visualComponents(false, false, true);
	rope->prepareRope(30822, ropeZoom.x, ropeZoom.y, 45);
	mRootNode->addChild(rope);

	std::vector<Actor*> text = rope->labels(niceTextSpacing, niceTextScale);
	for (Actor* a : text)
	{
		mRootNode->addChild(a);
	}

	TextData* textData = new TextData(TextData::Dynamic);
	const glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
	textData->set("Ropes", origin, fontHeight, niceTextSpacing * 10.0f, niceTextScale);
	textData->prepare();
	mRootNode->addChild(textData);
	LightSource* ls = new LightSource(glm::vec3(160.0f, 60.0f, 50.0f));
	//RendererBase* lightSource = NMS::createLightSource(glm::vec3(160.0f, 60.0f, 50.0f));
	//RendererBase* lightSource = NMS::createLightSource(glm::vec3(60.0f, 60.0f, -150.0f));
	ls->prepare();
	mRootNode->addChild(ls);
}

void NMSRopeScenePhysics::variableTimeStep(OWUtils::Time::duration OW_UNUSED(dt))
{}

void NMSRopeScenePhysics::fixedTimeStep(std::string& OW_UNUSED(nextSceneName),
	OWUtils::Time::duration OW_UNUSED(dt))
{}

void NMSRopeScenePhysics::interpolateRatio(const ScenePhysicsState* OW_UNUSED(previousState),
	double OW_UNUSED(multPrev),
	const ScenePhysicsState* OW_UNUSED(currentState),
	double OW_UNUSED(multCurr))
{}

void NMSRopeScenePhysics::copy(ScenePhysicsState* source)
{
	*this = *(dynamic_cast<const NMSRopeScenePhysics*>(source));
}

ScenePhysicsState* NMSRopeScenePhysics::clone()
{
	return new NMSRopeScenePhysics(owner());
}

bool NMSRopeScenePhysics::processUserCommands(const UserInput::AnyInput& userInput,
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
			const AABB& _world = NMSScene::world();
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

NMSRopeScene::NMSRopeScene(const Movie* _movie)
	: NMSScene(_movie)
{
	//	const glm::uvec2& screen = globals->physicalWindowSize();
}

void NMSRopeScene::doSetup(ScenePhysicsState* state)
{
	mRootNode = state->mRootNode;
}

void NMSRopeScene::render(const ScenePhysicsState* OW_UNUSED(state),
	const glm::mat4& proj, const glm::mat4& view,
	const glm::vec3& cameraPos)
{
	glm::mat4 model(1.0f);
	mRootNode->render(proj, view, model, cameraPos, nullptr, nullptr, nullptr);
}

void NMSRopeScene::activate(const std::string& OW_UNUSED(previousScene),
	ScenePhysicsState* state,
	Camera* camera, unsigned int callCount)
{
	NMSRopeScenePhysics* sp = dynamic_cast<NMSRopeScenePhysics*>(state);
	if (!callCount)
	{
		SceneGraphNode* rope = mRootNode->findChild("Ropes");
		AABB b = rope->bounds();
		glm::vec3 center = b.center();
		center.z = -200;
		camera->position(center);
		center.z = 0;
		camera->lookAt(center);
		float speed = camera->moveScale();
		camera->moveScale(speed * 5.0f);
		//camera->FOV(glm::radians(45.0f));
	}
}

void NMSRopeScene::deActivate(const Camera* OW_UNUSED(camera),
	ScenePhysicsState* OW_UNUSED(state))
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
