#include "NMSSplashScene.h"

#include <chrono>
#include <vector>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Cameras/Camera.h>
#include <Core/ErrorHandling.h>
#include <Core/GLApplication.h>
#include <Core/GlobalSettings.h>
#include <Core/Movie.h>
#include <Core/OWActor.h>
#include <Core/OcTree.h>
#include <Geometry/Particle.h>
#include <Geometry/Plane.h>
#include <Geometry/Box.h>
#include <Core/MeshComponent.h>

#include <Helpers/FreeTypeFontAtlas.h>
#include <Geometry/GeometricShapes.h>
#include <Helpers/ModelData.h>
#include <Helpers/ModelFactory.h>
#include <Helpers/ThreeDAxis.h>
#include <Helpers/Button.h>
#include <Helpers/Shader.h>
#include <Helpers/TextData.h>

#include "NMSUserInput.h"
#include "NMSRopeScene.h"

//#define INCLUDE_FULLSCREEN
//#define INCLUDE_WELCOME
//#define INCLUDE_ENJOY
#define INCLUDE_XYZ_AXIS
//#define INCLUDE_STAR_RENDER
//#define INCLUDE_IMPORTED_MODEL
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
AABB NMSSplashScenePhysics::mWindowBounds;

// We want the text to cross the screen (screenX = -1 -> screenX = 1) in 5 seconds. So 2 in 5 seconds 
// is a velocity of 0.4 per second
std::vector<OWMovableComponent*> addToOcTree;
OWUtils::Float NMSSplashScenePhysics::mSpeed;
static unsigned int gOctTreeThreshhold = 4;
static unsigned int gOctTreeMaxDepth = 5;

OWActor* mScenery = nullptr;
OcTree* mOctTree = nullptr;
/*
std::vector<OWActor*> mRootNode;
void traverseSceneGraph(Scene::OWActorCallbackType cb)
{
	for (OWActor* a : mRootNode)
	{
		cb(a);
	}
}
*/


void makeGlobals(Scene* owner)
{
	if (mScenery == nullptr)
	{
		mScenery = new OWActor(owner, glm::vec3(0));
		mScenery->name("Scenery");
		mOctTree = new OcTree();
	}
}

NMSSplashScenePhysics::NMSSplashScenePhysics(Scene* owner)
	: NMSWorldPhysicsState(owner)
{
	makeGlobals(owner);
}

void NMSSplashScenePhysics::clear() 
{
}

void NMSSplashScenePhysics::variableTimeStep(OWUtils::Time::duration dt)
{
	std::string dummy;
	fixedTimeStep(dummy, dt);
}

Box* bbp = nullptr;
static int ii = 10000;
static int off = 100;
void NMSSplashScenePhysics::fixedTimeStep(std::string& OW_UNUSED(nextSceneName),
	OWUtils::Time::duration dt)
{
	// Make the bounds a bit bigger than where the planes are.
	float bf = 1.2f;
	AABB planeBounds(glm::vec3(-off * bf), glm::vec3(off * bf));
	mOctTree->build(addToOcTree, gOctTreeThreshhold, gOctTreeMaxDepth, planeBounds);
	OWUtils::Float timeStep = std::chrono::duration<float>(dt).count();
	auto ticker = [timeStep](OWActor* a)
		{
			a->tick(timeStep, OWActor::TickType::InitialTick);
		};
	if (ii == 5000)
	{
		if (bbp)
			bbp->scale(glm::vec3(2));
	}
	if (ii == 1000)
	{
		if (bbp)
			bbp->rotate(45, { 1,1,1 });
	}
	if (ii > 0)
	{
	}
	//ii--;
	owner()->traverseSceneGraph(ticker);

	auto collider = [dt](OcTree* o)
		{
			int inc = 0;
			for (int i = 0; i < o->mPoints.size(); i++)
			{
				inc++;
				OWMovableComponent* a1 = o->mPoints[i];
				if (a1->canCollide())
				{
					if (a1->name() == "box")
					{
						char s = 'a';
					}
					for (int j = inc; j < o->mPoints.size(); j++)
					{
						OWMovableComponent* a2 = o->mPoints[j];
						if (a2->canCollide())
						{
							if (a1->canCollide(a2))
							{
								if (a1->bounds().intersects(a2->bounds()))
								{
									if (a1->collides(a2))
									{
										a1->collided(a2);
										a2->collided(a1);
									}
								}
							}
						}
					}
				}
			}
			return true;
		};
	mOctTree->traverse(collider);
#ifdef INCLUDE_WELCOME
//	mWelcomeMover.move(velocity);
	//mWelcomeMover.bounceIfCollide(mWindowBounds);
	//mWelcome->translate(mWelcomeMover.translateVector());
#endif
#ifdef INCLUDE_ENJOY
//	mEnjoyMover.move(velocity);
	//mEnjoyMover.bounceIfCollide(mWindowBounds);
	//mEnjoy->translate(mEnjoyMover.translateVector());
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

Box* createBox(const std::string& _name, const glm::vec4& colour, const glm::vec3& origin, const glm::vec3& direction, float speed, const glm::vec3& scale)
{
	Box* box = new Box(mScenery, origin);
	box->name(_name);
	box->prepare(colour);
	box->scale({ 10,10,10 });
	box->velocity(direction, speed);
	//box->rotate
	return box;
}

Plane* createBumperPlane(const std::string& _name, const glm::vec3& pos, float scale, float rotDegrees, const glm::vec3& rotAxis)
{
	Plane* p = new Plane(mScenery, pos);
	p->name(_name);
	glm::vec4 colour({ 1.0f, 0.33f, 0.33f, 0.2f });
	p->prepare(colour);
	p->scale(glm::vec3(scale));
	p->rotate(rotDegrees, rotAxis);
	return p;
}

void NMSSplashScenePhysics::setup()
{
	const AABB& _world = NMSScene::world();
	mWindowBounds = _world;
	mSpeed = _world.size().x / 200.0f;

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
	owner()->mRootNode.push_back(mScenery);
	glm::vec2 scale = { 1.2f * _world.size().x / globals->physicalWindowSize().x,
						1.2f * _world.size().y / globals->physicalWindowSize().y };
	const glm::vec3 origin = { 0.0f, 0.0f, 0.0f };

#ifdef INCLUDE_WELCOME
	
	glm::vec3 direction1 = Compass::Rose[Compass::North] +
		Compass::Rose[Compass::East] +
		Compass::Rose[Compass::In];
	TextData* welcome = new TextData(mScenery, glm::vec3(0), TextData::Dynamic);
	welcome->velocity(direction1, mSpeed);
	welcome->font("arial.ttf", fontHeight);
	welcome->colour({ 0.0, 0.0, 0.0, 1.0f });
	welcome->spacing(10 * nice.x, 10 * nice.y, scale);
	welcome->text("Welcome to reality.");
	welcome->prepare();
	addToOcTree.push_back(welcome);
#endif
	glm::vec3 scale1 = { 10, 10, 10 };
	glm::vec3 scale2 = { 3, 3, 3 };
	glm::vec3 scale3 = { 20, 20, 20 };
	glm::vec3 randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box1", OWUtils::colour(OWUtils::SolidColours::RED), randorigin, { 0.34, 0.57, 0.821 }, mSpeed * 0.8, scale1));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box2", OWUtils::colour(OWUtils::SolidColours::BLUE), randorigin, { 0.14, 0.27, 0.81 }, mSpeed * 0.8, scale1));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box3", OWUtils::colour(OWUtils::SolidColours::WHITE), randorigin, { 0.34, 0.21, 0.57 }, mSpeed * 0.8, scale1));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box3", OWUtils::colour(OWUtils::SolidColours::BRIGHT_CYAN), randorigin, { 0.34, 0.21, 0.57 }, mSpeed * 0.8, scale1));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box3", OWUtils::colour(OWUtils::SolidColours::MAGENTA), randorigin, { 0.34, 0.21, 0.57 }, mSpeed * 0.8, scale1));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box3", OWUtils::colour(OWUtils::SolidColours::YELLOW), randorigin, { 0.34, 0.21, 0.57 }, mSpeed * 0.8, scale1));

	addToOcTree.push_back(createBox("box1", OWUtils::colour(OWUtils::SolidColours::RED), randorigin, { 0.34, 0.57, 0.821 }, mSpeed * 1.8, scale2));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box2", OWUtils::colour(OWUtils::SolidColours::BLUE), randorigin, { 0.14, 0.27, 0.81 }, mSpeed * 3.8, scale2));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box3", OWUtils::colour(OWUtils::SolidColours::WHITE), randorigin, { 0.34, 0.21, 0.57 }, mSpeed * 0.1, scale3));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box3", OWUtils::colour(OWUtils::SolidColours::BRIGHT_CYAN), randorigin, { 0.34, 0.21, 0.57 }, mSpeed * 0.3, scale3));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box3", OWUtils::colour(OWUtils::SolidColours::MAGENTA), randorigin, { 0.34, 0.21, 0.57 }, mSpeed * 0.5, scale3));
	randorigin = { rand() % 50, rand() % 50 , rand() % 50 };
	addToOcTree.push_back(createBox("box3", OWUtils::colour(OWUtils::SolidColours::YELLOW), randorigin, { 0.34, 0.21, 0.57 }, mSpeed * 0.8, scale2));

	//glm::vec3 direction11 = Compass::Rose[Compass::North] + Compass::Rose[Compass::West];
	//box->rotate
//	bbp = box;

#ifdef INCLUDE_ENJOY
	glm::vec3 direction2 = Compass::Rose[Compass::South] +
		Compass::Rose[Compass::West];
	TextData* enjoy = new TextData(mScenery, glm::vec3(0), TextData::Static);
	enjoy->velocity(direction2, mSpeed);
	enjoy->font("arial.ttf", fontHeight);
	enjoy->colour({ 0.1, 0.9, 0.1, 1.0 });
	enjoy->spacing(nice.x, nice.y, scale);
	enjoy->text("Enjoy it while you can");
	enjoy->prepare();
	addToOcTree.push_back(enjoy);
#endif

#ifdef INCLUDE_IMPORTED_MODEL
	MeshComponent* dice = new MeshComponent(mScenery, glm::vec3(0));
	dice->name("Dice");
	ModelData md = ModelFactory().create("Dice2.obj", false);
	Shader* shader = new Shader("meshTest.v.glsl", "meshTest.f.glsl", "");
	shader->setStandardUniformNames("pvm");
	glm::vec3 scaleBy = glm::vec3(10.0, 10.0, 10.0);
	auto mm = [scaleBy](const glm::mat4& model)
		{
			return glm::scale(model, scaleBy);
		};
	dice->scale(glm::vec3(10.0, 10.0, 10.0));
	dice->setup(md.children[0].meshes[0], shader, GL_TRIANGLES, 0);
	addToOcTree.push_back(dice);
#endif
#ifdef INCLUDE_STAR_RENDER
	mButtonData.mButtonShape = GeometricShapes::goldenRectangle(10);
	mButtonData.mText = mEnjoyData;
	mButtonData.mText.text("Click Me");
#endif
	const float pos = off / 2.1f;
	// Create a box of planes for the objects to bounce off
	addToOcTree.push_back(createBumperPlane("Plane Front", glm::vec3(0, 0, pos), off, 0.0f, glm::vec3(1, 0, 0))); // Compass::In
	addToOcTree.push_back(createBumperPlane("Plane Back", glm::vec3(0, 0, -pos), off, 0.0f, glm::vec3(1, 0, 0))); // Compass::Out
	addToOcTree.push_back(createBumperPlane("Plane East", glm::vec3(pos, 0, 0), off, 90.0f, glm::vec3(0, 1, 0))); // Compass::East
	addToOcTree.push_back(createBumperPlane("Plane West", glm::vec3(-pos, 0, 0), off, 90.0f, glm::vec3(0, 1, 0))); // Compass::West
	addToOcTree.push_back(createBumperPlane("Plane North", glm::vec3(0, pos, 0), off, 90.0f, glm::vec3(1, 0, 0))); // Compass::North
	addToOcTree.push_back(createBumperPlane("Plane South", glm::vec3(0, -pos, 0), off, 90.0f, glm::vec3(1, 0, 0))); // Compass::Bottom
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

#ifdef INCLUDE_IMPORTED_MODEL
#endif
#ifdef INCLUDE_XYZ_AXIS
	ThreeDAxis* axis = new ThreeDAxis(this, world().center());
	axis->createAxisData(world());
	mRootNode.push_back(axis);

#endif
#ifdef INCLUDE_FULLSCREEN
	MeshComponent* fullScreen = new MeshComponent(mScenery, glm::vec3(0));
	fullScreen->name("Fullscreen");
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
		RenderTypes::ScaleByAspectRatioType scaleByAspectRatio,
		float OW_UNUSED(aspectRatio))
		{
			glm::vec2 vv = globals->physicalWindowSize();
			shader->setVector2f("u_resolution", vv);
		};
	sh->appendMutator(fullScreenRender);
	sh->appendResizer(fullScreenResize);
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
	auto rend = [proj, view, model, cameraPos](OWActor* a)
		{
			a->render(proj, view, model, cameraPos);
		};
	traverseSceneGraph(rend);
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

