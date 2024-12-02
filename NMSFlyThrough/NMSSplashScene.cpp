#include "NMSSplashScene.h"

#include <chrono>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Core/Camera.h>
#include <Core/ErrorHandling.h>
#include <Core/GLApplication.h>
#include <Core/GlobalSettings.h>
#include <Core/Movie.h>
#include <Core/MeshActor.h>
#include <Core/Actor.h>

#include <Helpers/FreeTypeFontAtlas.h>
#include <Helpers/GeometricShapes.h>
#include <Helpers/ModelData.h>
#include <Helpers/ModelFactory.h>
#include <Helpers/ThreeDAxis.h>
#include <Helpers/Button.h>


#include "NMSUserInput.h"
#include "NMSRopeScene.h"

#define INCLUDE_FULLSCREEN
#define INCLUDE_WELCOME
#define INCLUDE_ENJOY
#define INCLUDE_XYZ_AXIS
//#define INCLUDE_STAR_RENDER
#define INCLUDE_IMPORTED_MODEL
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
AABBV3 NMSSplashScenePhysics::mWindowBounds;

// We want the text to cross the screen (screenX = -1 -> screenX = 1) in 5 seconds. So 2 in 5 seconds 
// is a velocity of 0.4 per second
OWUtils::Float NMSSplashScenePhysics::mSpeed;

void NMSSplashScenePhysics::clear() 
{
}

void NMSSplashScenePhysics::variableTimeStep(OWUtils::Time::duration dt)
{
	std::string dummy;
	fixedTimeStep(dummy, dt);
}

void NMSSplashScenePhysics::fixedTimeStep(std::string& OW_UNUSED(nextSceneName),
	OWUtils::Time::duration dt)
{
	OWUtils::Float timeStep = std::chrono::duration<float>(dt).count();

	// Find the translation magnitudes
	glm::vec3 velocity = glm::vec3(timeStep * mSpeed, timeStep * mSpeed, timeStep * mSpeed);
#ifdef INCLUDE_WELCOME
	mWelcomeMover.move(velocity);
	mWelcomeMover.bounceIfCollide(mWindowBounds);
	mWelcome->translate(mWelcomeMover.translateVector());
#endif
#ifdef INCLUDE_ENJOY
	mEnjoyMover.move(velocity);
	mEnjoyMover.bounceIfCollide(mWindowBounds);
	mEnjoy->translate(mEnjoyMover.translateVector());
#endif
}

void NMSSplashScenePhysics::interpolateRatio(
					const ScenePhysicsState* OW_UNUSED(previousState), 
					double OW_UNUSED(multPrev),
					const ScenePhysicsState* OW_UNUSED(currentState), 
					double OW_UNUSED(multCurr))
{}

void NMSSplashScenePhysics::copy(ScenePhysicsState* source)
{
	*this = *(dynamic_cast<const NMSSplashScenePhysics*>(source));
}

ScenePhysicsState* NMSSplashScenePhysics::clone()
{
	return new NMSSplashScenePhysics(owner());
}

bool NMSSplashScenePhysics::processUserCommands(const UserInput::AnyInput& userInput,
					std::string& nextScene, 
					Camera* camera)
{
	float speed = camera->moveScale();
	if (speed < 201.00f)
		camera->moveScale(speed * 5.0f);
	if (userInput.inputType == UserInput::AnyInputType::Pointing)
	{
//		glm::ivec2 v2 = 
	//	glfwGetWindowSize(win, &screen_w, &screen_h); // better use the callback and cache the values 
		//glfwGetFramebufferSize(win, &pixel_w, &pixel_h); // better use the callback and cache the values 
	}
	else if (userInput.inputType == UserInput::AnyInputType::KeyPress)
	{
		// Keyboard
		NMSUserInput::LogicalOperator input = userInput.keyInput.userCommand;
		if (input == NMSUserInput::LogicalOperator::OptionsScreen)
		{
			nextScene = Scene::finalSceneName();
			return true;
		}
		if (input == NMSUserInput::LogicalOperator::RopeScreen)
		{
			nextScene = NMSRopeScene(nullptr).name();
			return true;
		}
		if (input == NMSUserInput::LogicalOperator::Accept)
		{
			if ((userInput.keyInput.mods | UserInput::InputMod::Shift) != 0)
			{
				nextScene = NMSScene::finalSceneName();
			}
			else
			{
				nextScene = NMSScene::finalSceneName();
			}
			return true;
		}
	}
	return false;
}

void NMSSplashScenePhysics::setup()
{
	const AABBV3& _world = NMSScene::world();
	mWindowBounds = _world;
	mSpeed = _world.size().x / 10.0f;

#ifdef INCLUDE_FULLSCREEN
#endif
#ifdef INCLUDE_STAR_RENDER
	mStarRadius = { 40.0, 40.0 };
	std::vector<glm::vec3> vertices = 
//		GeometricShapes::star(mStarRadius.x/5.0f, mStarRadius.x/3.3f, 15);
	GeometricShapes::rectangle(mStarRadius * 2.0f, -mStarRadius);
	mStarData.vertices(vertices, GL_TRIANGLES, 0);

	std::vector<glm::vec3> starPositions;
	starPositions.push_back({ 0,0,0 });
	starPositions.push_back({ 100,0,0 });
	starPositions.push_back({ 0,100,0 });
	starPositions.push_back({ 0,0,100 });
	mStarData.positions(starPositions, 1, 1);

	std::vector<glm::vec4> instanceColours;
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::BLUE));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::GREEN));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::YELLOW));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::RED));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::BRIGHT_MAGENTA));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::CYAN));
	mStarData.colours(instanceColours, 1, 2);
#endif

	int fontHeight = 24;
	glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
		fontHeight, globals->camera()->aspectRatio());

#ifdef INCLUDE_WELCOME
	glm::vec2 scale = { 1.2f * _world.size().x / globals->physicalWindowSize().x,
						1.2f * _world.size().y / globals->physicalWindowSize().y };
	const glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
	mWelcome = new TextData(TextData::Dynamic);
	mWelcome->font("arial.ttf", fontHeight);
	mWelcome->colour({ 0.0, 0.0, 0.0, 1.0f });
	mWelcome->spacing(10 * nice.x, 10 * nice.y, scale);
	mWelcome->text("Welcome to reality.");
	mWelcome->prepare();
	mRootNode->addChild(mWelcome);
#endif

#ifdef INCLUDE_ENJOY
	mEnjoy = new TextData(TextData::Static);
	mEnjoy->font("arial.ttf", fontHeight);
	mEnjoy->colour({ 0.1, 0.9, 0.1, 1 });
	mEnjoy->spacing(nice.x, nice.y, scale);
	mEnjoy->text("Enjoy it while you can");
	mEnjoy->prepare();
	mRootNode->addChild(mEnjoy);
#endif

#ifdef INCLUDE_IMPORTED_MODEL
	MeshActor* am = new MeshActor(nullptr);
	am->mName = "Dice";
	ModelData md = ModelFactory().create("Dice2.obj", false);
	Shader* shader = new Shader("meshTest.v.glsl", "meshTest.f.glsl", "");
	shader->setStandardUniformNames("pvm");
	am->setup(md.children[0].meshes[0], shader, GL_TRIANGLES, 0);
	glm::vec3 scaleBy = glm::vec3(10.0, 10.0, 10.0);
	am->scale(scaleBy);
	am->readyForRender();
	mRootNode->addChild(am);
#endif
#ifdef INCLUDE_STAR_RENDER
	mButtonData.mButtonShape = GeometricShapes::goldenRectangle(10);
	mButtonData.mText = mEnjoyData;
	mButtonData.mText.text("Click Me");
#endif
}

////////////////////////////////////// NMSSplashScene /////////////////////////////////////////////
NMSSplashScene::NMSSplashScene(const Movie* movie)
	: NMSScene(movie)
{
}

void NMSSplashScene::doSetup(ScenePhysicsState* state)
{
	NMSSplashScenePhysics* sps 
		= dynamic_cast<NMSSplashScenePhysics*>(state);
	mRootNode = state->mRootNode;

#ifdef INCLUDE_WELCOME

	sps->mWelcomeMover.setPosition(NMSScene::world().center());
	sps->mWelcomeMover.targetGeometry(sps->mWelcome->bounds(), glm::vec3(0.0f, 0.0f, 0.0f));
	sps->mWelcomeMover.direction(Compass::Rose[Compass::North] +
								Compass::Rose[Compass::East] +
								Compass::Rose[Compass::In]);
#endif
#ifdef INCLUDE_ENJOY

	sps->mEnjoyMover.setPosition(NMSScene::world().center());
	sps->mEnjoyMover.targetGeometry(sps->mEnjoy->bounds(), glm::vec3(0.0f, 0.0f, 0.0f));
	sps->mEnjoyMover.direction(Compass::Rose[Compass::South] +
		Compass::Rose[Compass::West]);
#endif

#ifdef INCLUDE_IMPORTED_MODEL
#endif
#ifdef INCLUDE_XYZ_AXIS
	ThreeDAxis* axis = new ThreeDAxis(nullptr);
	axis->createAxisData(world());
	mRootNode->addChild(axis);

#endif
#ifdef INCLUDE_FULLSCREEN
	MeshActor* fullScreen = new MeshActor(nullptr);
	fullScreen->mName = "Fullscreen";
	Shader* sh = new Shader("thebookofshaders.v.glsl",
		"thebookofshaders.f.glsl",
		"thebookofshaders_square.g.glsl");
	sh->setStandardUniformNames("pvm");
	std::vector<glm::vec4> pts;
	pts.push_back({ 0.0f, 0.0f, 0.0f, 0.0f });
	fullScreen->setup(pts, sh, GL_POINTS, 0);
	auto fullScreenRender = [](
		const glm::mat4& OW_UNUSED(proj),
		const glm::mat4& OW_UNUSED(view),
		const glm::mat4& OW_UNUSED(model),
		const glm::vec3& OW_UNUSED(cameraPos),
		const Shader* shader)
		{
			shader->setVector2f("u_mouse", globals->pointingDevicePosition());
			shader->setFloat("u_time", globals->secondsSinceLoad());
		};
	auto fullScreenResize = [](const Shader* shader,
		RendererBase::ScaleByAspectRatioType scaleByAspectRatio,
		float OW_UNUSED(aspectRatio))
		{
			glm::vec2 vv = globals->physicalWindowSize();
			shader->setVector2f("u_resolution", vv);
		};
	fullScreen->appendRenderCallback(fullScreenRender);
	fullScreen->appendResizeCallback(fullScreenResize);
	fullScreen->readyForRender();
	mRootNode->addChild(fullScreen);
#endif

#ifdef INCLUDE_STAR_RENDER
	Shader* starShader = new Shader(
		"instanced.v.glsl",
		"glow.f.glsl",
		//"oneLight.f.glsl",
		//"lightInSmoke.f.glsl"
		//"instanced.f.glsl",
		""// instanced.g.glsl
	);
	starShader->setStandardUniformNames("VP");
	glm::vec2 w = sps->mStarRadius;
	mStarRenderer = new InstanceRenderer(starShader);

	auto pointRender = [w](glm::mat4& OW_UNUSED(proj), glm::mat4& view,
		glm::mat4& OW_UNUSED(model), const Shader* shader)
	{
		glm::vec3 CameraRight_worldspace = { view[0][0], view[1][0], view[2][0] };
		shader->use();
		shader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
		glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
		shader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
		shader->setFloat("u_time", globals->secondsSinceLoad());
		glm::vec2 v2 = globals->pointingDevicePosition();
		shader->setVector2f("u_mouse", v2);
		shader->setVector2f("u_resolution", glm::vec2(w));
	};

	starShader->setFloat("cutoffRadius", w.x, true);
	mStarRenderer->setup(&(sps->mStarData));
	//mStarRenderer->blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	mStarRenderer->blendFunction(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	mButton = new OWButton();
	mButton->setup(sps->mButtonData, glm::vec3(100));
#endif
}

void NMSSplashScene::render(const ScenePhysicsState* state,
							const glm::mat4& proj, const glm::mat4& view,
							const glm::vec3& cameraPos)
{
	glm::mat4 model(1.0);
#ifdef INCLUDE_FULLSCREEN
#endif

#ifdef INCLUDE_STAR_RENDER
	glm::vec2 w = globals->physicalWindowSize();
	auto pointRender = [w](
		const glm::mat4& OW_UNUSED(proj), 
		const glm::mat4& view,
		const glm::mat4& OW_UNUSED(model), 
		const glm::vec3& OW_UNUSED(cameraPos), 
		const Shader* shader)
	{
		glm::vec3 CameraRight_worldspace = { view[0][0], view[1][0], view[2][0] };
		shader->use();
		shader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
		glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
		shader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
		shader->setFloat("u_time", globals->secondsSinceLoad());
		glm::vec2 v2 = globals->pointingDevicePosition();
		shader->setVector2f("u_mouse", v2);
		shader->setVector2f("u_resolution", w);
	};
	mStarRenderer->render(proj, view, model, cameraPos, nullptr, pointRender);
	mButton->render(proj, view, model, cameraPos);
#endif
	mRootNode->render(proj, view, model, cameraPos);
}

void NMSSplashScene::activate(const std::string& OW_UNUSED(previousScene), 
							  ScenePhysicsState* OW_UNUSED(state),
							  Camera* OW_UNUSED(camera),
							  unsigned int OW_UNUSED(callCount))
{
	//globals->application()->backgroundColour(glm::vec4(0, 0, 0, 1));
}

void NMSSplashScene::deActivate(const Camera* OW_UNUSED(camera), 
								ScenePhysicsState* OW_UNUSED(state))
{
	globals->application()->restoreBackgroundColour();
}

