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
#include <Core/SoundManager.h>
#include <Core/ResourcePathFactory.h>
#include <Core/LogStream.h>

#include <Core/CollisionSystem.h>
#include <Actor/ThreeDAxis.h>
#include <Actor/Button.h>
#include <Component/PhysicalComponent.h>
#include <Component/TextComponent.h>
#include <Component/MeshComponent.h>
#include <Component/ModelComponent.h>
#include <Geometry/GeometricShapes.h>
#include <Renderers/MeshRenderer.h>
#include <Renderers/ModelRenderer.h>

#include <Helpers/Collider.h>
#include <Helpers/Transform.h>
#include <Helpers/FreeTypeFontAtlas.h>
#include <Helpers/ModelFactory.h>
#include <Helpers/Shader.h>

#include "NMSUserInput.h"
#include "NMSRopeScene.h"

//#define INCLUDE_RAY
//#define INCLUDE_PLANES
//#define INCLUDE_FULLSCREEN
//#define INCLUDE_WELCOME
//#define INCLUDE_ENJOY
int GDEBUG_PICKING = 1;
//#define BOXES_CENTERED
//#define INCLUDE_XYZ_AXIS
//#define INCLUDE_STAR_RENDER
//#define INCLUDE_IMPORTED_MODEL
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
AABB NMSSplashScenePhysics::mWindowBounds;
// We want the text to cross the screen (screenX = -1 -> screenX = 1) in 5 seconds. 
// So 2 in 5 seconds 
// is a velocity of 0.4 per second
OWUtils::Float NMSSplashScenePhysics::mSpeed;
OWActorDiscrete* gRay = nullptr;

NMSSplashScenePhysics::NMSSplashScenePhysics(Scene* owner)
	: NMSWorldPhysicsState(owner)
{
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
#ifdef INCLUDE_RAY
			if (gRay != nullptr)
			{
				// this will crash
				// need to deactivate, remove from scene and remove from Collissions
				delete gRay;
			}
			gRay = new OWActorDiscrete(this->owner(), "Ray Actor");
			gRay->transform(new OWTransform(nullptr));
			OWActorDiscrete::DiscreteEntity sse;
			sse.colour = OWUtils::colour(OWUtils::SolidColours::RED);
			sse.coll = new OWCollider(gRay, OWCollider::CollisionType::Ray);
			sse.mesh = (new OWMeshComponent(gRay, "Ray Component"))
				->add(MeshData()
					.addVertices(OWGeometricShapes::beam(cam_pos, dir, 1000)));
			sse.rend = new OWMeshRenderer("", 
				{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour },
				GPUBufferObject::BufferStyle::SSBO);
			sse.trans = new OWTransform(gRay->transform(), cam_pos);
			gRay->addComponents(sse);
#endif
			gRay->transform()->localPosition(cam_pos);
			//gRay->colour({ 0.7, 0.7, 0.0, 1.0f });
			//gRay->direction(normMouse);
			glm::vec3 normal;
			float distance;
			bool intersects = false;
			glm::vec3 intersectPoint(0);
			/*
			for (auto& a : this->owner()->traverseSceneGraph
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
			*/
		}
	}
	if (userInput.mouseInput.action == UserInput::PointingDeviceAction::RightMouseButtonClick)
	{
		globals->application()->setWindowSize(glm::vec2(-1, 0));
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

OWActorMutableParticle::MutableParticleElement createBox(const std::string& _name, const glm::vec4& colour,
	const glm::vec3& origin, const glm::vec3& direction, float speed, const glm::vec3& scale)
{
	OWActorMutableParticle::MutableParticleElement elm;
	elm.colour = colour;
	elm.coll = new OWCollider(nullptr, OWCollider::CollisionType::Ovoid);
	elm.trans = new OWTransform(nullptr, origin, scale);
	elm.trans->rotation(glm::radians(45.0f), glm::vec3(1, 0, 0));
	OWPhysicsData pd;
	pd.velocity = direction * speed;
	elm.phys = new OWPhysics(pd);
	return elm;
/*
	bcd->shaderData.uniforms.push_back({ShaderDataUniforms::UV4F, "lightColor",
				OWUtils::to_string(OWUtils::colour(OWUtils::SolidColours::WHITE)) });
	bcd->shaderData.uniforms.push_back({ ShaderDataUniforms::UV4F, "objectColor", 
				OWUtils::to_string(colour) });
	bcd->shaderData.uniforms.push_back({ ShaderDataUniforms::UV3F, "viewLightPos", 
				OWUtils::to_string(glm::vec3(160.0f, 60.0f, 50.0f)) });
*/
}

OWActorMutableParticle::MutableParticleElement createBumperPlane(const std::string& _name, const glm::vec3& pos,
							float scale, float rotDegrees, const glm::vec3& rotAxis)
{
	OWActorMutableParticle::MutableParticleElement elm;
	elm.coll = new OWCollider(nullptr, OWCollider::CollisionType::Plane);
	elm.trans = new OWTransform(nullptr);
	elm.trans->rotation(glm::radians(rotDegrees), rotAxis);
	elm.trans->scale(glm::vec3(scale));
	elm.trans->localPosition(pos);
	elm.colour = OWUtils::randomSolidColour();
	elm.phys = new OWPhysics();
	return elm;
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
	OWActorNCom1Ren* dynamicTextActor = new OWActorNCom1Ren(this->owner(), "Dynamic Text Actor");
	dynamicTextActor->transform(glm::vec3(0), glm::vec3(scale, 1.0));

	OWPhysicsData pd1;
	pd1.velocity = Compass::Rose[Compass::North] +
				Compass::Rose[Compass::East] +
				Compass::Rose[Compass::In] * mSpeed;

	OWTextComponentData welcomeData;
	welcomeData.tdt = OWTextComponentData::TextDisplayType::Dynamic;
	welcomeData.fontName = "arial.ttf";
	welcomeData.fontHeight = fontHeight;
	welcomeData.colour = { 0.0, 0.0, 0.0, 1.0f };
	welcomeData.fontSpacing = { 10 * nice.x, 10 * nice.y };
	welcomeData.text = "Welcome to reality.";
	{
		OWActorNCom1Ren::NCom1RenElement elm;
		elm.coll = new OWCollider(dynamicTextActor, OWCollider::CollisionType::Box);
		elm.mesh = new OWTextComponent(dynamicTextActor, "Welcome", welcomeData);
		elm.phys = new OWPhysics(pd1);
		elm.sound = new OWSoundComponent();
		elm.trans = new OWTransform(nullptr, glm::vec3(0), glm::vec3(scale, 2.0));
		dynamicTextActor->renderer(new OWMeshRenderer("DynamicText.json", 
			{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour },
			GPUBufferObject::BufferStyle::SSBO));
		dynamicTextActor->addComponents(elm);
	}

#endif
	//Ray* r = new Ray(mScenery, { 20,20,20 }, { 1,1,1 });
	//r->prepare({ 0.0, 1.0, 0.0, 1.0f });
#ifdef INCLUDE_ENJOY
	OWActorNCom1Ren* staticTextActor = new OWActorNCom1Ren(this->owner(), "Static Text Actor");
	staticTextActor->transform(glm::vec3(0), glm::vec3(scale, 1.0));
	OWPhysicsData pd2;
	pd2.velocity = Compass::Rose[Compass::South] +
		Compass::Rose[Compass::West] * mSpeed / 20.0f;

	OWTextComponentData enjoyData;
	enjoyData.tdt = OWTextComponentData::TextDisplayType::Static;
	enjoyData.fontName = "arial.ttf";
	enjoyData.fontHeight = fontHeight;
	enjoyData.colour = { 0.1, 0.9, 0.1, 1.0 };
	enjoyData.fontSpacing = { nice.x, nice.y };
	enjoyData.text = "Enjoy it while you can.";

	{
		OWActorNCom1Ren::NCom1RenElement elm;
		elm.coll = new OWCollider(staticTextActor, OWCollider::CollisionType::Box);
		elm.mesh = new OWTextComponent(staticTextActor, "Enjoy", enjoyData);
		elm.phys = new OWPhysics(pd2);
		elm.sound = new OWSoundComponent();
		elm.trans = new OWTransform(nullptr, glm::vec3(0), glm::vec3(scale, 3.0));
		staticTextActor->renderer(new OWMeshRenderer("StaticText.json",
				{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour },
			GPUBufferObject::BufferStyle::SSBO));
		staticTextActor->addComponents(elm);
	}
#endif

	OWActorMutableParticle* boxActor = new OWActorMutableParticle(this->owner(), "All Boxes");
	boxActor->transform(new OWTransform(nullptr));
	boxActor->scriptor(new OWScriptComponent());
	boxActor->sound(new OWSoundComponent());
	MeshData mds1;
	boxActor->meshComponent(
		(new OWMeshComponent(boxActor, "Box Template"))
		->add(MeshData()
			.addVertices(OWGeometricShapes::cube())
			.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_FILL)));

	boxActor->renderer(new OWMeshRenderer("BoxShader.json", 
				{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour },
		GPUBufferObject::BufferStyle::SSBO));

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
		boxActor->addComponents(createBox("box1", OWUtils::colour(OWUtils::SolidColours::RED), 
							ro, rs, mSpeed * 0.0f, scale2));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
#ifdef BOXES_CENTERED
		ro = glm::vec3(0, 100, 0);
#endif
		boxActor->addComponents(createBox("box2", OWUtils::colour(OWUtils::SolidColours::BLUE), 
							ro, rs, mSpeed * 0.8f, scale1));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
#ifdef BOXES_CENTERED
		ro = glm::vec3(0, 0, 100);
#endif
		boxActor->addComponents(createBox("box3", OWUtils::colour(OWUtils::SolidColours::WHITE), 
							ro, rs, mSpeed * 0.8f, scale1));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
#ifdef BOXES_CENTERED
		ro = glm::vec3(0, 0, 0);
#endif
		boxActor->addComponents(createBox("box4", OWUtils::colour(OWUtils::SolidColours::BRIGHT_CYAN), 
							ro, rs, mSpeed * 0.8f, scale1));
#ifdef BOXES_CENTERED
		break;
#endif
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		boxActor->addComponents(createBox("box5", OWUtils::colour(OWUtils::SolidColours::MAGENTA), 
							ro, rs, mSpeed * 0.8f, scale1));

		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		boxActor->addComponents(createBox("box6", OWUtils::colour(OWUtils::SolidColours::YELLOW), 
							ro, rs, mSpeed * 0.8f, scale1));

		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		boxActor->addComponents(createBox("box7", OWUtils::colour(OWUtils::SolidColours::RED),
							ro, { rs }, mSpeed * 1.8f, scale2));

		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		boxActor->addComponents(createBox("box8", OWUtils::colour(OWUtils::SolidColours::BLUE), 
							ro, { rs }, mSpeed * 3.8f, scale2));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		boxActor->addComponents(createBox("box9", OWUtils::colour(OWUtils::SolidColours::WHITE), 
							ro, { rs }, mSpeed * 0.1f, scale3));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		boxActor->addComponents(createBox("box10", OWUtils::colour(OWUtils::SolidColours::BRIGHT_CYAN), 
							ro, { rs }, mSpeed * 0.3f, scale3));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		boxActor->addComponents(createBox("box11", OWUtils::colour(OWUtils::SolidColours::MAGENTA), 
							ro, { rs }, mSpeed * 0.5f, scale3));
		ro = { rand() % denom, rand() % denom , rand() % denom };
		rs = { rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f };
		boxActor->addComponents(createBox("box12", OWUtils::colour(OWUtils::SolidColours::YELLOW), 
							ro, { rs }, mSpeed * 0.8f, scale2));
	}

#ifdef INCLUDE_IMPORTED_MODEL
	OWActorDiscrete* singleModelActor = new OWActorDiscrete(this->owner(), "Dice");
	OWActorDiscrete::DiscreteEntity sse;
	sse.colour = sse.colour = OWUtils::colour(OWUtils::SolidColours::RED);
	sse.coll = new OWCollider(singleModelActor, OWCollider::CollisionType::Box);
	sse.mesh = new OWModelComponent(singleModelActor, "Dice Component", "Dice2.obj");
	sse.phys = new OWPhysics();
	sse.rend = new OWModelRenderer("DiceShader.json", { GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour });
	sse.sound = new OWSoundComponent();
	OWTransformData td;
	td.position = glm::vec3(0);
	td.scale = glm::vec3(10.0, 10.0, 10.0);
	sse.trans = new OWTransform(nullptr, td);
#endif
#ifdef INCLUDE_STAR_RENDER
	mButtonData.mButtonShape = GeometricShapes::goldenRectangle(10);
	mButtonData.mText = mEnjoyData;
	mButtonData.mText.text("Click Me");
#endif
	const float pos = off / 2.0f;
	// Create a box of planes for the objects to bounce off
#ifdef INCLUDE_PLANES
	OWActorMutableParticle* planeActor = new OWActorMutableParticle(this->owner(), "All Planes");
	planeActor->transform(new OWTransform(nullptr));
	planeActor->scriptor(new OWScriptComponent());
	planeActor->sound(new OWSoundComponent());
	std::vector<glm::vec3> v3 = OWGeometricShapes::rectangle(glm::vec2(1));
	v3[0].z -= 0.01f;
	v3[1].z += 0.01f;
	planeActor->meshComponent((new OWMeshComponent(planeActor, "Plane Template"))
		->add(MeshData()
			.setModes(GL_TRIANGLES, GL_TRIANGLES, GL_FILL)
			.addVertices(v3)));
	planeActor->renderer(new OWMeshRenderer("PlaneShader.json",
		{ GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour },
		GPUBufferObject::BufferStyle::SSBO));

	planeActor->addComponents(createBumperPlane("Plane Front", 
		glm::vec3(0, 0, pos), off, 0.0f, glm::vec3(1, 0, 0))); // Compass::In
	planeActor->addComponents(createBumperPlane("Plane Back", 
		glm::vec3(0, 0, -pos), off, 0.0f, glm::vec3(1, 0, 0))); // Compass::Out
	planeActor->addComponents(createBumperPlane("Plane East", 
		glm::vec3(pos, 0, 0), off, 90.0f, glm::vec3(0, 1, 0))); // Compass::East
	planeActor->addComponents(createBumperPlane("Plane West", 
		glm::vec3(-pos, 0, 0), off, 90.0f, glm::vec3(0, 1, 0))); // Compass::West
	planeActor->addComponents(createBumperPlane("Plane North", 
		glm::vec3(0, pos, 0), off, 90.0f, glm::vec3(1, 0, 0))); // Compass::North
	planeActor->addComponents(createBumperPlane("Plane South", 
		glm::vec3(0, -pos, 0), off, 90.0f, glm::vec3(1, 0, 0))); // Compass::Bottom
#endif
}

////////////////////////////////////// NMSSplashScene /////////////////////////////////////////////
NMSSplashScene::NMSSplashScene(const Movie* movie)
	: NMSScene(movie)
{
}

void NMSSplashScene::doSetupScene(ScenePhysicsState* state)
{
	NMSSplashScenePhysics* sps 
		= dynamic_cast<NMSSplashScenePhysics*>(state);

#ifdef INCLUDE_XYZ_AXIS
	OWThreeDAxisData axisData;
	axisData.bounds = AABB(glm::vec3(-100, -100, -100), glm::vec3(100, 100, 100));
	axisData.labelColour = OWUtils::colour(OWUtils::SolidColours::BRIGHT_YELLOW);
	ThreeDAxis* axis = new ThreeDAxis(this, "3D Axis");
	axis->initialise(axisData);

#endif
#ifdef INCLUDE_FULLSCREEN
	Shader* shader = new Shader("thebookofshaders.v.glsl",
		"thebookofshaders.f.glsl",
		"thebookofshaders_square.g.glsl");
	shader->setStandardUniformNames("pvm");
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
		auto fullScreenResize = [](
			const glm::mat4& OW_UNUSED(proj),
			const glm::mat4& OW_UNUSED(view),
			const glm::mat4& OW_UNUSED(model),
			const glm::vec3& OW_UNUSED(cameraPos),
			const Shader* shader)
		{
			glm::vec2 vv = globals->physicalWindowSize();
			shader->setVector2f("u_resolution", vv);
		};
	shader->appendMutator(fullScreenRender);
	shader->appendMutator(fullScreenResize);
	OWActorDiscrete* fullScreenActor = new OWActorDiscrete(this, "Fullscreen");
	MeshData mds;
	mds.v4.push_back({ 0.0f, 0.0f, 0.0f, 0.0f });
	mds.setPolygonMode(GL_FILL);
	OWActorDiscrete::DiscreteEntity sse;
	sse.colour = OWUtils::colour(OWUtils::SolidColours::RED);
	sse.coll = new OWCollider(fullScreenActor, OWCollider::CollisionType::Permeable);
	OWMeshComponent* mc = new OWMeshComponent(fullScreenActor, "Fullscreen Component");
	mc->add(mds);
	sse.mesh = mc;
	sse.phys = new OWPhysics();
	sse.rend = new OWModelRenderer(shader, { GPUBufferObject::BufferType::Position, GPUBufferObject::BufferType::Colour });
	sse.sound = new OWSoundComponent();
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

void NMSSplashScene::doRenderScene(const ScenePhysicsState* state,
							const glm::mat4& proj, const glm::mat4& view,
							const glm::vec3& cameraPos)
{
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

