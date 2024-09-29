#include "NMSSplashScene.h"

#include <chrono>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Core/Camera.h>
#include <Core/ErrorHandling.h>
#include <Core/GLApplication.h>
#include <Core/GlobalSettings.h>
#include <Core/Movie.h>

#include <Helpers/FreeTypeFontAtlas.h>
#include <Helpers/GeometricShapes.h>
#include <Helpers/ModelData.h>
#include <Helpers/ModelFactory.h>
#include <Helpers/Shader.h>
#include <Helpers/ShaderFactory.h>

#include <Renderers/HeavyRenderer.h>
#include <Renderers/InstanceRenderer.h>
#include <Renderers/LightRenderer.h>
#include <Renderers/TextRendererDynamic.h>
#include <Renderers/TextRendererStatic.h>

#include "NMSUtils.h"
#include "NMSUserInput.h"
#include "NMSRopeScene.h"

#define INCLUDE_FULLSCREEN
#define INCLUDE_WELCOME
#define INCLUDE_ENJOY
#define INCLUDE_XYZ_AXIS
#define INCLUDE_STAR_RENDER
#define INCLUDE_IMPORTED_MODEL
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
AABB NMSSplashScenePhysics::mWindowBounds;

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
	glm::vec4 velocity = glm::vec4(timeStep * mSpeed, timeStep * mSpeed, timeStep * mSpeed, 1.0);
#ifdef INCLUDE_WELCOME
	mWelcomeMover.move(velocity);
	mWelcomeMover.bounceIfCollide(mWindowBounds);
#endif
#ifdef INCLUDE_ENJOY
	mEnjoyMover.move(velocity);
	mEnjoyMover.bounceIfCollide(mWindowBounds);
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
					Camera* OW_UNUSED(camera))
{
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
	const AABB& _world = NMSScene::world();
	mWindowBounds = _world;
	mSpeed = _world.size().x / 10.0f;

#ifdef INCLUDE_FULLSCREEN
	mFullScreenData.push_back({ 0.0f, 0.0f, 0.0f, 0.0f });
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
	mWelcomeData.font("arial.ttf", fontHeight);
	mWelcomeData.colour({ 0.0, 0.0, 0.0, 1.0f });
	mWelcomeData.spacing(10 * nice.x, 10 * nice.y, scale);
	mWelcomeData.text("Welcome to reality.");
#endif

#ifdef INCLUDE_ENJOY
	mEnjoyData.font("arial.ttf", fontHeight);
	mEnjoyData.colour({ 0.1, 0.9, 0.1, 1 });
	mEnjoyData.spacing(nice.x, nice.y, scale);
	mEnjoyData.text("Enjoy it while you can");
#endif

#ifdef INCLUDE_IMPORTED_MODEL
	ModelData md = ModelFactory().create("Dice2.obj", false);
	mCylinderData = md.children[0].meshes[0];
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

#ifdef INCLUDE_WELCOME
	mWelcomeText = new TextRendererDynamic();
	mWelcomeText->setup(&sps->mWelcomeData, glm::vec3(0));

	sps->mWelcomeMover.setPosition(NMSScene::world().center());
	sps->mWelcomeMover.targetGeometry(mWelcomeText->bounds(), glm::vec3(0.0f, 0.0f, 0.0f));
	sps->mWelcomeMover.direction(Compass::Rose[Compass::North] +
								Compass::Rose[Compass::East] +
								Compass::Rose[Compass::In]);
#endif
#ifdef INCLUDE_ENJOY
	mEnjoyText = new TextRendererStatic();
	mEnjoyText->setup(&sps->mEnjoyData, glm::vec3(0));

	sps->mEnjoyMover.setPosition(NMSScene::world().center());
	sps->mEnjoyMover.targetGeometry(mEnjoyText->bounds(), glm::vec3(0.0f, 0.0f, 0.0f));
	sps->mEnjoyMover.direction(Compass::Rose[Compass::South] +
		Compass::Rose[Compass::West]);
#endif

#ifdef INCLUDE_IMPORTED_MODEL
	Shader* shader = new Shader("meshTest.v.glsl", "meshTest.f.glsl", "");
	shader->setStandardUniformNames("pvm");
	mCylinder = new HeavyRenderer(shader);
	mCylinder->setup(sps->mCylinderData, GL_TRIANGLES, 0);
#endif
#ifdef INCLUDE_XYZ_AXIS
	ModelData md = NMS::createAxisData(world());
	mAxis.setup(&md);
#endif
#ifdef INCLUDE_FULLSCREEN
	Shader* sh = new Shader("thebookofshaders.v.glsl",
		"thebookofshaders.f.glsl",
		"thebookofshaders_square.g.glsl");
	sh->setStandardUniformNames("pvm");
	mFullScreen = new LightRenderer(sh);
	mFullScreen->setup(sps->mFullScreenData, GL_POINTS);
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
	mStarRenderer->setup(&sps->mStarData);
	//mStarRenderer->blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	mStarRenderer->blendFunction(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif
}

void NMSSplashScene::render(const ScenePhysicsState* state,
							const glm::mat4& proj, const glm::mat4& view,
							const glm::vec3& cameraPos)
{
	const NMSSplashScenePhysics* sps
		= dynamic_cast<const NMSSplashScenePhysics*>(state);
	glm::mat4 model(1.0);
#ifdef INCLUDE_FULLSCREEN
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
	mFullScreen->render(proj, view, model, cameraPos,
					nullptr, fullScreenRender, fullScreenResize);
#endif
#ifdef INCLUDE_IMPORTED_MODEL
	glm::vec3 scaled = glm::vec3(10.0, 10.0, 10.0);
	glm::mat4 scaledModel2 = glm::scale(model, scaled);
	mCylinder->render(proj, view, scaledModel2, cameraPos, nullptr, nullptr);
#endif
#ifdef INCLUDE_WELCOME
	mWelcomeText->render(proj, view, sps->mWelcomeMover.translate(model), cameraPos);
#endif
#ifdef INCLUDE_ENJOY
	mEnjoyText->render(proj, view, sps->mEnjoyMover.translate(model), cameraPos);
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
#endif

#ifdef INCLUDE_XYZ_AXIS
	mAxis.render(proj, view, model, cameraPos);
#endif
}

void NMSSplashScene::activate(const std::string& OW_UNUSED(previousScene), 
							  ScenePhysicsState* OW_UNUSED(state),
							  Camera* OW_UNUSED(camera), 
							  unsigned int OW_UNUSED(callCount))
{
	//globals->application()->backgroundColour(glm::vec4(0, 0, 0, 0));
}

void NMSSplashScene::deActivate(const Camera* OW_UNUSED(camera), 
								ScenePhysicsState* OW_UNUSED(state))
{
	globals->application()->restoreBackgroundColour();
}

