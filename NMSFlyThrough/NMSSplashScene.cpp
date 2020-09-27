#include "NMSSplashScene.h"

#include <chrono>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <GLFW/glfw3.h>

#include <Core/Movie.h>

#include <Helpers/Shader.h>
#include <Core/Camera.h>
#include <Helpers/ErrorHandling.h>
#include <Renderables/TextBillboardFixed.h>
#include <Renderables/TextBillboardDynamic.h>
#include <Renderables/Axis.h>
#include <Renderables/FullScreen.h>
#include <Renderables/Circle.h>
#include <Core/GLApplication.h>

#include "NMSUserInput.h"

#define INCLUDE_WELCOME
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
	mWelcome.move(velocity);
	mWelcome.bounceIfCollide(mWindowBounds);
#endif
	mEnjoy.move(velocity);
	mEnjoy.bounceIfCollide(mWindowBounds);
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
	: NMSScene(movie)
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
	{
		TextBillboard* welcomeText = new TextBillboardDynamic("arial.ttf", fontHeight);

		welcomeText->createText("Welcome to reality.", 10 * nice.x, 10 * nice.y);
		welcomeText->colour({ 0.0, 0.0, 0.0, 1.0f }, "textcolor");
		sps->mWelcome.text(welcomeText);
		glm::vec2 scale = { 1.2f * _world.size().x / globals->physicalWindowSize().x,
							1.2f * _world.size().y / globals->physicalWindowSize().y };
		welcomeText->scale(scale);
		sps->mWelcome.direction(Compass::Rose[Compass::North] + Compass::Rose[Compass::East]
									+ Compass::Rose[Compass::In]);
		sps->mWelcome.setPosition(NMSScene::world().center());
	}
#endif
	{
		TextBillboard* enjoyFontAtlas = new TextBillboardFixed("arial.ttf", fontHeight);
		enjoyFontAtlas->createText("Enjoy it while you can", nice.x, nice.y);
		enjoyFontAtlas->colour({ 0.1, 0.9, 0.1, 1 }, "textcolor");
		sps->mEnjoy.text(enjoyFontAtlas);
		sps->mEnjoy.direction(Compass::Rose[Compass::South] + Compass::Rose[Compass::West]); //South West
		sps->mEnjoy.setPosition(NMSScene::world().center());
	}
	{
		FullScreen* fs = new FullScreen(new Shader("thebookofshaders.v.glsl",
			"thebookofshaders.f.glsl",
			"thebookofshaders_square.g.glsl"), "pvm");
		fs->setUp(_world);
		mFullScreen.addResizer(new ResizeHelper());
		mFullScreen.addSource(fs);
	}
	{
		mAxis = new Axis();
		mAxis->setUp(_world, movie()->camera());
	}
	{
		Circle* circle = new Circle();
		circle->setUp();
		mCircle.addSource(circle);
	}
}

void NMSSplashScene::render(const ScenePhysicsState* state,
							const glm::mat4& proj, const glm::mat4& view)
{
	const AABB& _world = world();
	const NMSSplashScenePhysics* sps = dynamic_cast<const NMSSplashScenePhysics*>(state);
	glm::mat4 translation(1.0);
	glm::mat4 rotation(1.0);
	glm::mat4 model(1.0);
	glm::vec2 scale = { 20.2f * _world.size().x / globals->physicalWindowSize().x,
						20.2f * _world.size().y / globals->physicalWindowSize().y };
	mAxis->render(proj, view, model);
	mFullScreen.render(proj, view, model);

	sps->mEnjoy.render(proj, view, model);
	mCircle.render(proj, view, model);
#ifdef INCLUDE_WELCOME
	glm::mat4 m = glm::scale(model, glm::vec3(scale, 0.0));
	sps->mWelcome.render(proj, view, m);
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

