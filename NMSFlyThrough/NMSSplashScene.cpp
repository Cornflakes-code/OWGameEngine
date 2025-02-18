#include "NMSSplashScene.h"

#include <chrono>
#include <vector>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Cameras/Camera.h>
#include <Core/CommonUtils.h>
#include <Core/ErrorHandling.h>
#include <Core/GLApplication.h>
#include <Core/GlobalSettings.h>
#include <Core/Movie.h>
#include <Core/LogStream.h>

#include <Actor/StaticSceneryActor.h>
#include <Actor/CollisionSystem.h>
#include <Actor/OcTree.h>
#include <Actor/ThreeDAxis.h>
#include <Actor/Button.h>
#include <Component/BoxComponent.h>
#include <Component/PlaneComponent.h>
#include <Component/RayComponent.h>
#include <Component/MeshComponentHeavy.h>
#include <Component/MeshComponentLight.h>
#include <Component/TextComponent.h>
#include <Geometry/BoundingBox.h>

#include <Helpers/FreeTypeFontAtlas.h>
#include <Helpers/ModelData.h>
#include <Helpers/ModelFactory.h>
#include <Helpers/Shader.h>

#include "NMSUserInput.h"
#include "NMSRopeScene.h"

//#define INCLUDE_PLANES
//#define INCLUDE_FULLSCREEN
//#define INCLUDE_WELCOME
#define INCLUDE_ENJOY
int GDEBUG_PICKING = 4;
//#define BOXES_CENTERED
#define INCLUDE_XYZ_AXIS
//#define INCLUDE_STAR_RENDER
//#define INCLUDE_IMPORTED_MODEL
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
AABB NMSSplashScenePhysics::mWindowBounds;
BoxComponent* gBox = nullptr;
TextComponent* gWelcome = nullptr;
TextComponent* gEnjoy = nullptr;
std::vector<OWSceneComponent*> addToOcTree;
// We want the text to cross the screen (screenX = -1 -> screenX = 1) in 5 seconds. So 2 in 5 seconds 
// is a velocity of 0.4 per second
OWUtils::Float NMSSplashScenePhysics::mSpeed;

StaticSceneryActor* mScenery = nullptr;

void makeGlobals(Scene* owner)
{
	if (mScenery == nullptr)
	{
		StaticSceneryData* data = new StaticSceneryData();
		StaticSceneryScript* script = new StaticSceneryScript(data);
		mScenery = new StaticSceneryActor(owner, script);
//		mOctTree = new OcTree();
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
	OWUtils::Float timeStep = std::chrono::duration<float>(dt).count();
	//CollisionSystem::tick(timeStep);
	CollisionSystem::collide();
}

static constexpr float off = 500;
void NMSSplashScenePhysics::fixedTimeStep(std::string& OW_UNUSED(nextSceneName),
	OWUtils::Time::duration dt)
{
	// Make the bounds a bit bigger than where the planes are.
	float bf = 1.2f;
	AABB planeBounds(glm::vec3(-off * bf), glm::vec3(off * bf));
	CollisionSystem::refresh();
	OWUtils::Float timeStep = std::chrono::duration<float>(dt).count();
	//CollisionSystem::tick(timeStep);
	CollisionSystem::collide();
#ifdef INCLUDE_WELCOME
#endif
#ifdef INCLUDE_ENJOY
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
	//*this = *(dynamic_cast<const NMSSplashScenePhysics*>(source));
}

ScenePhysicsState* NMSSplashScenePhysics::clone()
{
	return new NMSSplashScenePhysics(owner());
}

bool NMSSplashScenePhysics::processUserCommands(const UserInput::AnyInput& userInput,
	std::string& nextScene,
	Camera* camera)
{
	if (userInput.inputType == UserInput::AnyInputType::Pointing)
	{
		//		glm::ivec2 v2 = 
			//	glfwGetWindowSize(win, &screen_w, &screen_h); // better use the callback and cache the values 
				//glfwGetFramebufferSize(win, &pixel_w, &pixel_h); // better use the callback and cache the values 
		if (userInput.mouseInput.action == UserInput::PointingDeviceAction::LeftMouseButtonClick)
		{
			glm::vec3 mousePos = globals->mouseToWorld(userInput.mouseInput.pos, false);
			LogStream(LogStreamLevel::Info) << "userInput.mouseInput Position " << userInput.mouseInput.pos << "\n";
			LogStream(LogStreamLevel::Info) << "MouseToWorld Position " << mousePos << "\n";
			glm::vec3 normMouse = glm::normalize(mousePos);
			glm::vec3 cam_pos = camera->position();
			glm::vec3 dir = cam_pos - mousePos;
			RayComponentData* rcd = new RayComponentData();
			rcd->colour = { 0.7, 0.7, 0.0, 1.0f };
			rcd->direction = normMouse;
			rcd->origin = cam_pos;
			RayComponent* r = new RayComponent(mScenery, rcd);
			r->init();
			glm::vec3 normal;
			float distance;
			bool intersects = false;
			glm::vec3 intersectPoint(0);
			for (auto& a : addToOcTree)
			{
				intersects = r->intersects(a->constData()->boundingBox, normal, distance);
				intersectPoint = cam_pos + normMouse * distance;
				LogStream(LogStreamLevel::Info) << "ray intersects [" << a->name() << "] position [" 
					<< a->constData()->physics.mTranslate << "] ["
					<< (intersects ? "true" : "false") << "] at [" << intersectPoint << "]\n";
			}
			if (gWelcome != nullptr)
			{
				const AABB& bb = gWelcome->constData()->boundingBox;
				intersects = r->intersects(bb, normal, distance);
				intersectPoint = cam_pos + normMouse * distance;
				LogStream(LogStreamLevel::Info) << "Welcome intersects ["
					<< (intersects ? "true" : "false") << "] at [" << intersectPoint << "] BB ["
					<< bb.maxPoint() << " : " << bb.minPoint() << "] \n";
			}
			if (gEnjoy != nullptr)
			{
				const AABB& bb = gEnjoy->constData()->boundingBox;
				intersects = r->intersects(bb, normal, distance);
				intersectPoint = cam_pos + normMouse * distance;
				LogStream(LogStreamLevel::Info) << "Enjoy intersects [" 
					<< (intersects ? "true" : "false") << "] at [" << intersectPoint << "] BB ["
					<< bb.maxPoint() << " : " << bb.minPoint() << "] \n";
			}
		}
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

BoxComponent* createBox(const std::string& _name, const glm::vec4& colour, 
	const glm::vec3& origin, const glm::vec3& direction, float speed, const glm::vec3& scale)
{
	BoxComponentData* bcd = new BoxComponentData();
	bcd->polygonMode = GL_FILL;
	bcd->shaderData.shaderV = "Wires.v.glsl";
	bcd->shaderData.shaderF = "Wires.f.glsl";
	bcd->shaderData.shaderG = "";
	bcd->shaderData.PVMName = "";
	bcd->shaderData.projectionName = "projection";
	bcd->shaderData.viewName = "view";
	bcd->shaderData.modelName = "model";
	bcd->shaderData.uniforms.push_back({ ShaderDataUniforms::UV4F, "lightColor", 
				OWUtils::to_string(OWUtils::colour(OWUtils::SolidColours::WHITE)) });
	bcd->shaderData.uniforms.push_back({ ShaderDataUniforms::UV4F, "objectColor", 
				OWUtils::to_string(colour) });
	bcd->shaderData.uniforms.push_back({ ShaderDataUniforms::UV3F, "viewLightPos", 
				OWUtils::to_string(glm::vec3(160.0f, 60.0f, 50.0f)) });
	bcd->name = _name;
	bcd->physics.velocity = direction * speed;
	bcd->physics.rotate(glm::radians(45.0f), glm::vec3(1,0,0));
	bcd->physics.scale(scale);
	bcd->physics.translate(origin);
	BoxComponent* box = new BoxComponent(mScenery, bcd);
	gBox = box;
	return box;
}

PlaneComponent* createBumperPlane(const std::string& _name, const glm::vec3& pos, 
							float scale, float rotDegrees, const glm::vec3& rotAxis)
{
	PlaneComponentData* pcd = new PlaneComponentData();
	pcd->colour = { 1.0f, 0.33f, 0.33f, 0.2f };
	glm::mat4 m(1.0f);
	pcd->physics.scale(glm::vec3(scale));
	pcd->physics.rotate(glm::radians(rotDegrees), rotAxis);
	pcd->physics.translate(pos);
	pcd->name = _name;
	pcd->canMove = false;
	PlaneComponent* p = new PlaneComponent(mScenery, pcd);
	p->init();
	return p;
}

void NMSSplashScenePhysics::setup()
{
	const AABB& _world = NMSScene::world();
	mWindowBounds = _world;
	mSpeed = _world.size().x / 100.0f;

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
	glm::vec2 scale = { 1.2f * _world.size().x / globals->physicalWindowSize().x,
						1.2f * _world.size().y / globals->physicalWindowSize().y };
	const glm::vec3 origin = { 0.0f, 0.0f, 0.0f };

#ifdef INCLUDE_WELCOME
	
	TextComponentData* welcomeData = new TextComponentData();
	welcomeData->textData.tdt = TextData::TextDisplayType::Dynamic;
	glm::vec3 velocity = Compass::Rose[Compass::North] +
		Compass::Rose[Compass::East] +
		Compass::Rose[Compass::In] * mSpeed;
	//welcomeData->physics.velocity = velocity;
	welcomeData->physics.translate(glm::vec3(0, 0, 0));
	welcomeData->textData.fontName = "arial.ttf";
	welcomeData->textData.fontHeight = fontHeight;
	welcomeData->textData.colour = { 0.0, 0.0, 0.0, 1.0f };
	welcomeData->textData.fontSpacing = { 10 * nice.x, 10 * nice.y };
	welcomeData->physics.scale(glm::vec3(scale, 1.0));
	//welcomeData->physics.scale(glm::vec3(10.0f));
	welcomeData->textData.text = "Welcome to reality.";
	TextComponent* welcome = new TextComponent(mScenery, welcomeData);
	addToOcTree.push_back(welcome);
	gWelcome = welcome;
#endif
	//Ray* r = new Ray(mScenery, { 20,20,20 }, { 1,1,1 });
	//r->prepare({ 0.0, 1.0, 0.0, 1.0f });
#ifdef INCLUDE_ENJOY
	TextComponentData* enjoyData = new TextComponentData();
	enjoyData->textData.tdt = TextData::TextDisplayType::Static;
	glm::vec3 velocity2 = Compass::Rose[Compass::South] +
		Compass::Rose[Compass::West] * mSpeed / 20.0f;
	//enjoyData->physics.velocity = velocity2;
	enjoyData->physics.translate(glm::vec3(0, 0, 0));
	enjoyData->textData.fontName = "arial.ttf";
	enjoyData->textData.fontHeight = fontHeight;
	enjoyData->textData.colour = { 0.1, 0.9, 0.1, 1.0 };
	enjoyData->textData.fontSpacing = { nice.x, nice.y };
	enjoyData->physics.scale(glm::vec3(scale, 1.0));
	enjoyData->textData.text = "Enjoy it while you can.";
	TextComponent* enjoy = new TextComponent(mScenery, enjoyData);
	//addToOcTree.push_back(enjoy);
	gEnjoy = enjoy;
#endif

	glm::vec3 scale1 = { 10, 10, 10 };
	glm::vec3 scale2 = { 3, 3, 3 };
	glm::vec3 scale3 = { 20, 20, 20 };
	int denom = 10;

	for (int i = 0; i < GDEBUG_PICKING; i++)
	{
		glm::vec3 ro = { rand() % denom, rand() % denom, rand() % denom }; // random origin
		glm::vec3 rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f }; // random speed
#ifdef BOXES_CENTERED
		ro = glm::vec3(100, 0, 0);
#endif
		addToOcTree.push_back(createBox("box1", OWUtils::colour(OWUtils::SolidColours::RED), ro, rs, mSpeed * 0.0f, scale2));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
#ifdef BOXES_CENTERED
		ro = glm::vec3(0, 100, 0);
#endif
		addToOcTree.push_back(createBox("box2", OWUtils::colour(OWUtils::SolidColours::BLUE), ro, rs, mSpeed * 0.8f, scale1));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
#ifdef BOXES_CENTERED
		ro = glm::vec3(0, 0, 100);
#endif
		addToOcTree.push_back(createBox("box3", OWUtils::colour(OWUtils::SolidColours::WHITE), ro, rs, mSpeed * 0.8f, scale1));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
#ifdef BOXES_CENTERED
		ro = glm::vec3(0, 0, 0);
#endif
		addToOcTree.push_back(createBox("box4", OWUtils::colour(OWUtils::SolidColours::BRIGHT_CYAN), ro, rs, mSpeed * 0.8f, scale1));
#ifdef BOXES_CENTERED
		break;
#endif
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		addToOcTree.push_back(createBox("box5", OWUtils::colour(OWUtils::SolidColours::MAGENTA), ro, rs, mSpeed * 0.8f, scale1));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		addToOcTree.push_back(createBox("box6", OWUtils::colour(OWUtils::SolidColours::YELLOW), ro, rs, mSpeed * 0.8f, scale1));

		addToOcTree.push_back(createBox("box7", OWUtils::colour(OWUtils::SolidColours::RED), ro, { rs }, mSpeed * 1.8f, scale2));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		addToOcTree.push_back(createBox("box8", OWUtils::colour(OWUtils::SolidColours::BLUE), ro, { rs }, mSpeed * 3.8f, scale2));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		addToOcTree.push_back(createBox("box9", OWUtils::colour(OWUtils::SolidColours::WHITE), ro, { rs }, mSpeed * 0.1f, scale3));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		addToOcTree.push_back(createBox("box10", OWUtils::colour(OWUtils::SolidColours::BRIGHT_CYAN), ro, { rs }, mSpeed * 0.3f, scale3));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		addToOcTree.push_back(createBox("box11", OWUtils::colour(OWUtils::SolidColours::MAGENTA), ro, { rs }, mSpeed * 0.5f, scale3));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		addToOcTree.push_back(createBox("box12", OWUtils::colour(OWUtils::SolidColours::YELLOW), ro, { rs }, mSpeed * 0.8f, scale2));
	}
	//glm::vec3 direction11 = Compass::Rose[Compass::North] + Compass::Rose[Compass::West];
	//box->rotate

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
	const float pos = off / 2.0f;
	// Create a box of planes for the objects to bounce off
#ifdef INCLUDE_PLANES
	createBumperPlane("Plane Front", glm::vec3(0, 0, pos), off, 0.0f, glm::vec3(1, 0, 0)); // Compass::In
	createBumperPlane("Plane Back", glm::vec3(0, 0, -pos), off, 0.0f, glm::vec3(1, 0, 0)); // Compass::Out
	createBumperPlane("Plane East", glm::vec3(pos, 0, 0), off, 90.0f, glm::vec3(0, 1, 0)); // Compass::East
	createBumperPlane("Plane West", glm::vec3(-pos, 0, 0), off, 90.0f, glm::vec3(0, 1, 0)); // Compass::West
	createBumperPlane("Plane North", glm::vec3(0, pos, 0), off, 90.0f, glm::vec3(1, 0, 0)); // Compass::North
	createBumperPlane("Plane South", glm::vec3(0, -pos, 0), off, 90.0f, glm::vec3(1, 0, 0)); // Compass::Bottom
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

#ifdef INCLUDE_XYZ_AXIS
	OWThreeDAxisData* axisData = new OWThreeDAxisData();
	AABB w = world();
	axisData->axisData.world = w;
	axisData->actorData.position = glm::vec3(0);// w.center();
	axisData->axisData.labelColour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_YELLOW);
	OWThreeDAxisScript* script = new OWThreeDAxisScript(axisData);
	ThreeDAxis* axis = new ThreeDAxis(this, script);

#endif
#ifdef INCLUDE_FULLSCREEN
	MeshComponentLightData* mcd = new MeshComponentLightData();
	mcd->name = "Fullscreen";
	
	mcd->shaderData.shaderV = "thebookofshaders.v.glsl";
	mcd->shaderData.shaderF = "thebookofshaders.f.glsl";
	mcd->shaderData.shaderG = "thebookofshaders_square.g.glsl";
	mcd->shaderData.PVMName = "pvm";
	std::vector<glm::vec4> pts;
	pts.push_back({ 0.0f, 0.0f, 0.0f, 0.0f });
	mcd->meshData.vertices(pts, GL_POINTS, 0);
	//fullScreen->renderBoundingBox(false);
	auto fullScreenRender = [](
		const glm::mat4& OW_UNUSED(proj),
		const glm::mat4& OW_UNUSED(view),
		const glm::mat4& OW_UNUSED(model),
		const glm::vec3& OW_UNUSED(cameraPos),
		const Shader* shader)
		{
			//shader->setVector2f("u_mouse", globals->pointingDevicePosition());
			shader->setFloat("u_time", globals->secondsSinceLoad());
		};
	auto fullScreenResize = [](const Shader* shader,
		RenderTypes::ScaleByAspectRatioType scaleByAspectRatio,
		float OW_UNUSED(aspectRatio))
		{
			glm::vec2 vv = globals->physicalWindowSize();
			shader->setVector2f("u_resolution", vv);
		};
	mcd->shaderData.mutatorCallbacks.push_back(fullScreenRender);
	mcd->shaderData.resizeCallbacks.push_back(fullScreenResize);
	MeshComponentLight* fullScreen = new MeshComponentLight(mScenery, mcd);
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
	auto init = [](OWActor* a)
		{
			a->init();
		};
	traverseSceneGraph(init);
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
							  Camera* camera,
							  unsigned int OW_UNUSED(callCount))
{
	//globals->application()->backgroundColour(glm::vec4(0, 0, 0, 1));
	//camera->position({ -39.9999f, 40, 240 });
	camera->position({ 0, 0, 200 });
	//camera->lookAt({ -39.758, 40, 239.029 });
	camera->lookAt({ 0,0,0 });
	float speed = camera->moveScale();
	if (speed < 201.00f)
		camera->moveScale(speed * 5.0f);
}

void NMSSplashScene::deActivate(const Camera* OW_UNUSED(camera), 
								ScenePhysicsState* OW_UNUSED(state))
{
	globals->application()->restoreBackgroundColour();
}

