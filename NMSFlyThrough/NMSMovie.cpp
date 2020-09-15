#include "NMSMovie.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/GLApplication.h>

#include <Core/Camera.h>
#include <Helpers/ErrorHandling.h>

#include "NMSSplashScene.h"
#include "NMSEndScene.h"
#include "NMSMainScene.h"
#include <Helpers/FreeTypeFontAtlas.h>

NMSMovie::NMSMovie(Camera* _camera)
	: Movie("NMS", glm::uvec2(800, 600), _camera)
{
}

void NMSMovie::init(GLApplication* app, UserInput* ui, MacroRecorder* recorder)
{
	Movie::init(app, ui, recorder);
}

void NMSMovie::preRun()
{
	Movie::preRun();
	// These are pleasing values for sx/sy parameters 
	// passed to createText. I cannot think where else to store these. 
	// Maybe could be in a config file loaded at start up?
	FreeTypeFontAtlas::FontDetails::pleasingSpacing(24, 0.00625f, 0.00625f * 2);
	FreeTypeFontAtlas::FontDetails::pleasingSpacing(12, 0.00625f, 0.00625f * 2);

	NMSScene* s = new NMSSplashScene(this);

	this->add(s, new NMSSplashScenePhysics(s), true);

	s = new NMSMainScene(this);

	this->add(s, new NMSMainScenePhysics(s), false);

	s = new NMSEndScene(this);

	this->add(s, new NMSEndScenePhysics(s), false);

	// Here we set up for our star display
	/*
	create 3D grid.
	create plane in center line of galaxy
	identify current star
	identify target star
	create stars plus labels
	create camera at current star looking a center of galaxy
	*/
	Camera* _camera = camera();
	_camera->aspectRatio(this->physicalWindowSize().x / 
						(1.0f * this->physicalWindowSize().y));
	_camera->moveScale(NMSScene::world().size().x / 1.0f);
	
	const glm::vec3& wmax = NMSScene::world().maxPoint();
	_camera->position(glm::vec3(0.01f, -wmax.y*2.2, 0));
	//_camera->position({ 0.001f, NMSScene::world().maxPoint().y * 0.01f, 0.001f });
	_camera->lookAt(NMSScene::world().center());
	//_camera->roll(3.141592653/4); // PI

	_camera->clipping(0.01f, 1200.0f);
	_camera->FOV(glm::radians(45.0f));
}


void NMSMovie ::render(const ScenePhysicsState* state)
{
	Movie::render(state);
}
