#include "NMSSplashScene.h"

#include <chrono>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Core/Movie.h>
#include <Core/GLApplication.h>
#include <Core/GlobalSettings.h>
#include <Core/Camera.h>

#include <Helpers/Shader.h>
#include <Helpers/ErrorHandling.h>
#include <Helpers/GeometricShapes.h>

#include <Renderables/TextBillboardFixed.h>
#include <Renderables/TextBillboardDynamic.h>
#include <Renderables/Axis.h>
#include <Renderables/SimpleModelRenderer.h>
#include <Renderables/ModelRenderer.h>
#include <Renderables/ParticlesRenderer.h>

#include "NMSUserInput.h"

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
		int input = userInput.keyInput.userCommand;
		if (input == NMSUserInput::BaseUserCommand::OptionsScreen)
		{
			nextScene = Scene::quitSceneName();
			return true;
		}
		if (input == NMSUserInput::BaseUserCommand::Accept)
		{
			nextScene = NMSScene::mainSceneName();
			return true;
		}
	}
	return false;
}
////////////////////////////////////// NMSSplashScene /////////////////////////////////////////////
NMSSplashScene::NMSSplashScene(const Movie* movie)
	: NMSScene(movie),
	mFullScreen(glm::vec3(0.0f, 0.0f, 0.0f),
		new Shader("thebookofshaders.v.glsl",
		"thebookofshaders.f.glsl",
		"thebookofshaders_square.g.glsl"), "pvm")
{
}

void NMSSplashScene::doSetup(ScenePhysicsState* state)
{
	const AABB& _world = world();
	NMSSplashScenePhysics::mWindowBounds = _world;
	NMSSplashScenePhysics::mSpeed = _world.size().x / 10.0f;

	NMSSplashScenePhysics* sps = dynamic_cast<NMSSplashScenePhysics*>(state);
	
	int fontHeight = 24;
	glm::vec2 nice = FreeTypeFontAtlas::FontDetails::pleasingSpacing(
							fontHeight, movie()->camera()->aspectRatio());
#ifdef INCLUDE_WELCOME
	glm::vec2 scale = { 1.2f * _world.size().x / globals->physicalWindowSize().x,
						1.2f * _world.size().y / globals->physicalWindowSize().y };
	mWelcomeText  = new TextBillboardDynamic(glm::vec3(0.0f, 0.0f, 0.0f),
											"arial.ttf", fontHeight);
	mWelcomeText->createText("Welcome to reality.", 10 * nice.x, 10 * nice.y);
	mWelcomeText->colour({ 0.0, 0.0, 0.0, 1.0f }, "textcolor");
	mWelcomeText->scale(scale);
	mWelcomeText->renderer(new SimpleModelRenderer());

	sps->mWelcomeMover.targetGeometry(mWelcomeText->bounds(), mWelcomeText->initialPosition());
	sps->mWelcomeMover.direction(Compass::Rose[Compass::North] +
							Compass::Rose[Compass::East] + 
							Compass::Rose[Compass::In]);
	sps->mWelcomeMover.setPosition(NMSScene::world().center());
#endif
#ifdef INCLUDE_ENJOY
	mEnjoyText = new TextBillboardFixed(glm::vec3(0.0f, 0.0f, 0.0f),
										"arial.ttf", 12);
	mEnjoyText->createText("Enjoy it while you can", nice.x, nice.y);
	mEnjoyText->colour({ 0.1, 0.9, 0.1, 1 }, "textcolor");
	//mEnjoyText->scale(scale);
	mEnjoyText->renderer(new SimpleModelRenderer());
	sps->mEnjoyMover.targetGeometry(mEnjoyText->bounds(), mEnjoyText->initialPosition());
	sps->mEnjoyMover.direction(Compass::Rose[Compass::South] +
							   Compass::Rose[Compass::West]);
#endif
#ifdef INCLUDE_FULLSCREEN
	mFullScreen.prepare(_world);
	mFullScreen.renderer(new SimpleModelRenderer());
#endif

#ifdef INCLUDE_XYZ_AXIS
	mAxis = new Axis();
	mAxis->prepare(_world);
	mCircle.prepare();
	mCircle.renderer(new SimpleModelRenderer());
#endif

	Shader* instanceShader = new Shader("instanced.v.glsl",
							"instanced.f.glsl",
		//					instanced.g.glsl
		""
	);
#ifdef INCLUDE_STAR_RENDER
	mStarRenderer.shader(instanceShader, "VP");
	std::vector<glm::vec3> squareVertices 
//		= GeometricShapes::rectangle(glm::vec2(1.0, 1.0), glm::vec2(-0.5, -0.5));
		= GeometricShapes::rectangle(glm::vec2(8.00, 8.0), glm::vec2(-4, -4));
	std::vector<glm::vec3> target;
	target.push_back({ 0,0,0 });
	mStarRenderer.vertices(squareVertices, 0, GL_TRIANGLES);
	mStarRenderer.positions(target, 1, 1, GL_POINTS);

	std::vector<glm::vec4> instanceColours;
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::YELLOW));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::GREEN));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::RED));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLUE));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::BRIGHT_MAGENTA));
	instanceColours.push_back(OWUtils::colour(OWUtils::SolidColours::CYAN));
	mStarRenderer.colours(instanceColours, 2, 1);
	mStarRenderer.renderer(new ParticlesRenderer());
#endif
#ifdef INCLUDE_IMPORTED_MODEL
	Shader* modelShader = new Shader("meshTest.v.glsl", "meshTest.f.glsl", "");
	mCylinder.shader(modelShader, "pvm");
//	mCylinder.create("Dice2.obj");
	mCylinder.create("Cylinder1.obj");
	mCylinder.renderer(new ModelRenderer());
#endif
}


void NMSSplashScene::render(const ScenePhysicsState* state,
							const glm::mat4& proj, const glm::mat4& view)
{
	const AABB& _world = world();
	glm::vec2 scalex = { 20.2f * _world.size().x / globals->physicalWindowSize().x,
						20.2f * _world.size().y / globals->physicalWindowSize().y };
		const NMSSplashScenePhysics* sps 
			= dynamic_cast<const NMSSplashScenePhysics*>(state);
	glm::mat4 model(1.0);
//	mCircle.render(proj, view, model);
#ifdef INCLUDE_FULLSCREEN
	mFullScreen.render(proj, view, model);
#endif
#ifdef INCLUDE_XYZ_AXIS
	mAxis->render(proj, view, model);
#endif
#ifdef INCLUDE_ENJOY
	mEnjoyText->render(proj, view, model, &sps->mEnjoyMover);
#endif
#ifdef INCLUDE_WELCOME
	glm::vec2 scale = { 20.2f * _world.size().x / globals->physicalWindowSize().x,
						20.2f * _world.size().y / globals->physicalWindowSize().y };
	//glm::mat4 scaledModel = glm::scale(model, glm::vec3(scale, 0.0));
	glm::mat4 scaledModel = model;
	mWelcomeText->render(proj, view, scaledModel, &sps->mWelcomeMover);
#endif
	auto pointRender = [](const glm::mat4& OW_UNUSED(proj), const glm::mat4& view,
		const glm::mat4& OW_UNUSED(model), const Shader* shader) {
		glm::vec3 CameraRight_worldspace =
		{ view[0][0], view[1][0], view[2][0] };
		shader->use();
		shader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
		glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
		shader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
		shader->setFloat("u_time", globals->secondsSinceLoad());
		glm::vec2 v2 = globals->pointingDevicePosition();
		shader->setVector2f("u_mouse", v2);
	};
#ifdef INCLUDE_STAR_RENDER
	mStarRenderer.render(proj, view, model, nullptr, pointRender);
#endif
#ifdef INCLUDE_IMPORTED_MODEL
	glm::vec3 scaled = glm::vec3(10.0, 10.0, 10.0);
	glm::mat4 scaledModel2 = glm::scale(model, scaled);
	mCylinder.render(proj, view, scaledModel2, nullptr, nullptr);
#endif
}

void NMSSplashScene::activate(const std::string& OW_UNUSED(previousScene), 
							  ScenePhysicsState* OW_UNUSED(state),
							  Camera* OW_UNUSED(camera), 
							  unsigned int OW_UNUSED(callCount))
{
}

void NMSSplashScene::deActivate(const std::string& OW_UNUSED(previousScene),
							const Camera* OW_UNUSED(camera), 
							ScenePhysicsState* OW_UNUSED(state))
{
}

